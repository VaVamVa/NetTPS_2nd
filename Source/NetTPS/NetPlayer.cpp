// Fill out your copyright notice in the Description page of Project Settings.

#include "NetPlayer.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "GameWidget.h"
#include "Gun.h"
#include "HPBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainWidget.h"
#include "NetGameState.h"
#include "NetPlayerState.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ANetPlayer::ANetPlayer()
{
	// 총 붙일 Component 생성
	compGun = CreateDefaultSubobject<USceneComponent>(TEXT("GUN"));
	compGun->SetupAttachment(GetMesh(), TEXT("weapon_l"));
	compGun->SetRelativeLocation(FVector(0, 7, 5.5f));

	// HPBar Widget Component
	compHP = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP"));
	compHP->SetupAttachment(RootComponent);
}

void ANetPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Level 에 있는 모든 총을 찾자.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGun::StaticClass(), allGun);
	
	// CameraBoom 초기 위치 설정
	CameraBoom->SetRelativeLocation(cameraBoomLocationWithoutGun);

	// 만약에 서버라면
	if (HasAuthority())
	{
		// Game State 가져오자.
		ANetGameState* gsb = Cast<ANetGameState>(GetWorld()->GetGameState());
		// 나를 추가하자.
		gsb->AddPlayer(this);
	}
}

void ANetPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 총 집기/ 놓기 Input 발생했을 때 호출되는 함수 등록
		enhancedInputComponent->BindAction(takeGunAction, ETriggerEvent::Started, this, &ANetPlayer::TakeGun);
		// 총 쏘기 Input 발생했을 때 호출되는 함수 등록
		enhancedInputComponent->BindAction(fireAction, ETriggerEvent::Started, this, &ANetPlayer::Fire);
		// 재장전 Input 발생했을 때 호출되는 함수 등록
		enhancedInputComponent->BindAction(reloadAction, ETriggerEvent::Started, this, &ANetPlayer::Reload);
	}
}

void ANetPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// 누르고 있을 때
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::J))
	{
		UE_LOG(LogTemp, Warning, TEXT("J 키 누르고 있음"));
	}
	// 눌렀을 때
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
    {
		MakeCube();
    }
	// 떼었을 때
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased(EKeys::J))
	{
		UE_LOG(LogTemp, Warning, TEXT("J 키 떼었을 때"));
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
	{
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		UEditableTextBox* textBox = GetWorld()->GetGameState<ANetGameState>()->gameWidget->chatInputBox;
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(playerController);
		textBox->SetKeyboardFocus();
	}

	BillboardHPBar();

	PrintNetLog();
}

void ANetPlayer::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate 할 변수 등록
	DOREPLIFETIME(ANetPlayer, ownedGun);
	DOREPLIFETIME(ANetPlayer, canMakeCube);
}

// 서버에서만 호출되는 함수
void ANetPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	FString isServer = HasAuthority() ? TEXT("[서버]") : TEXT("[클라]");
	UE_LOG(LogTemp, Warning, TEXT("%s - %s - %s"), *isServer, TEXT(__FUNCTION__), *GetActorNameOrLabel());

	ClinetRPC_OnPossess();
}

void ANetPlayer::TakeGun()
{
	// [서버] 에게 총 집기/놓기 요청
	ServerRPC_TakeGun();
}

void ANetPlayer::ServerRPC_TakeGun_Implementation()
{
	if (hasGun == false)
	{
		// 만약에 죽었으면 함수를 나가자.
		if (isDie) return;
		// 현재까지 최단거리(초기값은 엄청 큰값 or 최대 범위 값)
		float closestDist = canTakeGunDist;
		// 현재까지 최단거리 총의 index
		int32 closestIdx = -1;
		// Level 에 있는 모든 총들 중에
		for (int32 i = 0; i < allGun.Num(); i++)
		{
			// 만약에 총이 Owner 가 있다면 건너뛰자.
			if (allGun[i]->GetOwner() != nullptr) continue;
			// 나와 i 번째 총의 거리를 구하자.
			float dist = FVector::Distance(allGun[i]->GetActorLocation(), GetActorLocation());
			// 그 거리가 총을 집을 수 있는 거리가 아니라면 건너뛰자.
			if (dist > canTakeGunDist) continue;
			// 현재까지 최단거리 보다 작다면
			if (dist < closestDist)
			{
				// 현재까지 최단거리를 갱신
				closestDist = dist;
				// 현재까지 최단거리 총의 idx 갱신
				closestIdx = i;
			}
		}

		// 만약에 closestIdx 가 -1 이라면( canTakeGunDist 범위 밖에 모든 총이 존재)
		if (closestIdx != -1)
		{
			// cloestIdx 의 총을 ownedGun 설정
			ownedGun = Cast<AGun>(allGun[closestIdx]);
			// ownedGun 의 Owner 를 나로하자.
			ownedGun->SetOwner(this);
			// 검색 된 총을 나에게 붙이자.
			AttachGun();
		}
	}
	// 총을 들고 있고, 재장전 중이 아니라면
	else if (isReload == false)
	{
		// ownedGun 을 담을 임시변수
		AGun* tempGun = ownedGun;
		// ownedGun 의 Owner 를 nullptr 로 하자
		ownedGun->SetOwner(nullptr);
		// ownedGun 을 nullptr 로 설정
		ownedGun = nullptr;
		// 모든 [클라] 에게 총놔라!
		MulticastRPC_DetachGun(tempGun);
	}
}

void ANetPlayer::AttachGun()
{
	// 만약에 ownedGun 이 nullptr 함수나가자.
	if (ownedGun == nullptr) return;
	
	// 총을 집은 상태로 설정
	hasGun = true;
	// 물리적인 움직임 비활성
	UStaticMeshComponent* comp = ownedGun->GetComponentByClass<UStaticMeshComponent>();
	comp->SetSimulatePhysics(false);
	// compGun 에 총을 붙이자.
	ownedGun->AttachToComponent(compGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// 총 집었는지 여부에 따라 CameraBoom 설정 변경
	ChangeCameraBoomSetting();
	// 만약에 내 Player 이면
	if (IsLocallyControlled())
	{
		// 총알 UI를 총에 들어있는 총알 갯수만큼 채우자.
		mainUI->AddBullet(ownedGun->GetBulletCount());
		// Crosshair 활성
		mainUI->ShowCrosshair(true);
	}
}

void ANetPlayer::MulticastRPC_DetachGun_Implementation(class AGun* gun)
{
	DetachGun(gun);
}

void ANetPlayer::DetachGun(AGun* gun)
{
	// 총을 버린 상태로 설정
	hasGun = false;
	// 물리적인 움직임 활성
	UStaticMeshComponent* comp = gun->GetComponentByClass<UStaticMeshComponent>();
	comp->SetSimulatePhysics(true);
	// compGun 에서 총을 떼어내자.
	gun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// 총 집었는지 여부에 따라 CameraBoom 설정 변경
	ChangeCameraBoomSetting();
	// 공격 상태 초기화
	OnFireComplete();
	
	// 만약에 내 Player 라면
	if (IsLocallyControlled())
	{
		// 총알 UI 모두 지우자
		mainUI->PopBulletAll();
		// Crosshair 비활성
		mainUI->ShowCrosshair(false);
	}
}

void ANetPlayer::ChangeCameraBoomSetting()
{
	// 이동 방향으로 모델 돌아가게 하기 / 하지 않기
	GetCharacterMovement()->bOrientRotationToMovement = !hasGun;
	// 카메라 회전에 따라 캐릭터 Yaw 회전 하기 / 하지 않기
	bUseControllerRotationYaw = hasGun;
	// CameraBoom 위치 수정
	CameraBoom->SetRelativeLocation(hasGun ? cameraBoomLocationWithGun : cameraBoomLocationWithoutGun);
	// TargetArmLength 수정
	CameraBoom->TargetArmLength = hasGun ? targetArmLengthWithGun : targetArmLengthWithoutGun;
}

void ANetPlayer::Fire()
{
	ServerRPC_Fire();
}

void ANetPlayer::ServerRPC_Fire_Implementation()
{
	// 만약에 총을 들고있지 않으면 함수를 나가자.
	if (hasGun == false) return;
	// 총알이 없으면 함수를 나가자.
	if (ownedGun->GetBulletCount() <= 0) return;
	// 만약에 재장전 중이면 함수를 나가자.
	if (isReload) return;

	// 만약에 공격 중이라면
	if (isFire)
	{
		// 콤보 연결
		isCombo = true;
	}
	else
	{
		// 공격 상태로 설정
		isFire = true;
		// 공격 실행
		FireAction();
	}
}

void ANetPlayer::MulticastRPC_FireAction_Implementation(bool bHit,
	FHitResult hitInfo, int32 combo)
{
	// 총 쏘는 애니메이션 실행
	FString fireName = FString::Printf(TEXT("Fire_%d"), combo);
	PlayAnimMontage(playerMontage, 1.0f, FName(fireName));
	// 총알 갯수 하나 제거
	ownedGun->PopBullet();
	// 만약에 내 Player 라면
	if (IsLocallyControlled())
	{
		// 총알 UI 하나 제거
		mainUI->PopBullet();
	}

	// 어딘가에 부딪혔다면
	if (bHit)
	{
		// UKismetMathLibrary::GetReflectionVector()
		// 입사각
		FVector inVector = hitInfo.TraceEnd - hitInfo.TraceStart;
		// 법선벡터 (노멀벡터)
		FVector normalVector = hitInfo.Normal;
		// 반사각 계산
		float dot = FVector::DotProduct(inVector, normalVector);
		FVector outVector = inVector - 2 * dot * normalVector;
		// 반사각으로 파티클 효과가 재생되게 각도 구하자.
		FRotator rot = UKismetMathLibrary::MakeRotFromX(outVector);
		
		// 맞은 지점에 파티클 효과 표현
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect, hitInfo.Location, rot);

		// 만약 맞은 Actor 가 NetPlayer 라면
		if (ANetPlayer* player = Cast<ANetPlayer>(hitInfo.GetActor()))
		{
			// 데미지 처리 하자.
			player->DamageProcess(20);
			GetWorld()->GetGameState<ANetGameState>()->AddScore(GetPlayerState());
		}
	}
}

void ANetPlayer::FireAction()
{
	// 시작 지점
	FVector startPos = FollowCamera->GetComponentLocation();
	// 종료 지점
	FVector endPos = startPos + FollowCamera->GetForwardVector() * 100000;
	// 그 외 옵션
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	// 부딪혔을 때 그 정보를 담을 변수
	FHitResult hitInfo;
	// LineTrace 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	// 모든 [클라] 에게 LineTrace 결과 넘겨서 총쏘게 하자
	MulticastRPC_FireAction(bHit, hitInfo, comboCnt);
}

void ANetPlayer::Reload()
{
	ServerRPC_Reload();
}

void ANetPlayer::ServerRPC_Reload_Implementation()
{
	// 만약에 총을 들고있지 않으면 함수를 나가자.
	if (hasGun == false) return;
	// 총알이 가득 차 있다면 함수를 나가자.
	if (ownedGun->IsFillBullet()) return;
	// 만약에 재장전 중이면 함수를 나가자.
	if (isReload) return;
	// 만약에 공격 중이면 함수를 나가자.
	if (isFire) return;

	// 모든 [클라] 에게 재장전 애니메이션 해라
	MulticastRPC_Reload();
}

void ANetPlayer::MulticastRPC_Reload_Implementation()
{
	// 재장전 중으로 설정
	isReload = true;	
	// 재장전 애니메이션 실행
	PlayAnimMontage(playerMontage, 1.0f, FName(TEXT("Reload")));
}

void ANetPlayer::OnReloadComplete()
{
	// Die 상태면 함수 나가자
	if (isDie) return;
	// 재장전 끝 설정
	isReload = false;
	// 총알 가득 채우자.
	ownedGun->FillBullet();
	
	if (IsLocallyControlled())
	{
		// 총알 UI 가득 채우자
		mainUI->AddBullet(ownedGun->GetBulletCount());
	}
}

void ANetPlayer::DamageProcess(float damage)
{
	if (IsLocallyControlled())
	{
		mainUI->ShowDamageUI();
	}
	
	// 내 Player 라면 mainUI 에 있는 HPBar 를 설정
	// 그렇지 않으면 compHP 에 있는 HPBar 를 설정
	UHPBar* hpBar = IsLocallyControlled() ? mainUI->hpBarUI : Cast<UHPBar>(compHP->GetWidget());
	
	// 머리 위에 있는 HPBar 갱신
	float currHP = hpBar->UpdateHP(damage);
	
	// 죽었는지 여부 설정
	isDie = currHP <= 0;
	if (isDie) DieProcess();
}

void ANetPlayer::BillboardHPBar()
{
	// 내가 컨트롤하고 있는 카메라를 가져오자.
	AActor* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// 카메라의 앞 방향 (반대), 윗 방향을 이용해서 Rotator 를 구하자.
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-cam->GetActorForwardVector(), cam->GetActorUpVector());
	// 구한 Rotator 를 comHP 에 설정
	compHP->SetWorldRotation(rot);
}

void ANetPlayer::DieProcess()
{
	// 재장전 완료 상태로 바꾸자.
	isReload = false;
	// 움직이지 못하게 설정
	GetCharacterMovement()->DisableMovement();
	// 충돌되지 않게 설정
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	// 만약에 내 Player 라면
	if (IsLocallyControlled())
	{
		// 총을 들고 있다면 총 놓자.
		if (hasGun) TakeGun();
		// 화면 흑백 처리
		FollowCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		// MainUI 삭제
		mainUI->RemoveFromParent();
		// 다시하기 버튼 보이게
		ANetGameState* gs = Cast<ANetGameState>(GetWorld()->GetGameState());
		gs->gameWidget->ShowBtnRetry(true);
		// 마우스 보이게
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}

void ANetPlayer::OnCombo()
{
	// 콤보 키를 누르지 않았다면 함수를 나가자.
	if (isCombo == false) return;
	// 다음 콤보 체크를 위해서 isCombo false
	isCombo = false;
	// 콤보 카운트 증가
	comboCnt++;
	// 공격 액션
	FireAction();
}

void ANetPlayer::OnFireComplete()
{
	// 콤보 관련 변수 초기화
	isFire = isCombo = false;
	comboCnt = 0;
}



void ANetPlayer::PrintNetLog()
{
	// NetConnection 상태
	FString connStr = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");
	// Owner 상태
	FString ownerStr = GetOwner() != nullptr ? GetOwner()->GetActorNameOrLabel() : TEXT("Invalid Owner");
	// 내 것인지 / 남의 것인지
	FString mineStr = IsLocallyControlled() ? TEXT("내 것") : TEXT("남의 것");
	// role
	// LOCAL : 내 입장에 보는 Role
	// REMOTE : [서버] 에서는 이 액터가 클라이언트에서는 *** 으로 Role 설정되어있다.
	//			[클라] 에서는 이 액터가 서버에서는 *** 으로 Role 설정되어있다
	FString roleStr = FString::Printf(TEXT("LOCAL : %s, REMOTE : %s"),
		*UEnum::GetValueAsString<ENetRole>(GetLocalRole()),
		*UEnum::GetValueAsString<ENetRole>(GetRemoteRole()));
	
	FString logStr = FString::Printf(TEXT("Connection : %s\r\nOwner : %s\r\nMine : %s\r\nRole : %s"),
		*connStr, *ownerStr, *mineStr, *roleStr);

	DrawDebugString(GetWorld(), GetActorLocation(), logStr, nullptr, FColor::Yellow, 0, true);
}

void ANetPlayer::ClinetRPC_OnPossess_Implementation()
{	
	// Main UI 를 만들자.
	mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);
	mainUI->AddToViewport(-1);
	// 머리 위 HPBar 보이지 않게 설정
	compHP->SetVisibility(false);
	
	FString isServer = HasAuthority() ? TEXT("[서버]") : TEXT("[클라]");
	UE_LOG(LogTemp, Warning, TEXT("%s - %s - %s"), *isServer, TEXT(__FUNCTION__), *GetActorNameOrLabel());
}

void ANetPlayer::MakeCube()
{
	// 만약에 큐브 만들 수 없는 상태면 함수 나가자.
	if (canMakeCube == false) return;
	// 만약에 내 Player 가 아니면 함수 나가자.
	if (IsLocallyControlled() == false) return;
	// [서버] 에게 큐브 생성 요청
	ServerRPC_MakeCube();
}

void ANetPlayer::ServerRPC_MakeCube_Implementation()
{
	// 내 위치 기준 앞방향으로 300 만큼 떨어진 위치
	FVector pos = GetActorLocation() + GetActorForwardVector() * 300;
	// 큐브를 pos 위치, 내 회전값으로 설정해서 생성하자.
	GetWorld()->SpawnActor<AActor>(cubeFactory, pos, GetActorRotation());
	// 다음 큐브 만들 사람 정하자.
	ANetGameState* gsb = Cast<ANetGameState>(GetWorld()->GetGameState());
	gsb->ChangeTurn();
}















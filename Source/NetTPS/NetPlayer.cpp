// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayer.h"

#include "EnhancedInputComponent.h"
#include "Gun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainWidget.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"


ANetPlayer::ANetPlayer()
{
	// 총 붙일 Component 생성
	compGun = CreateDefaultSubobject<USceneComponent>(TEXT("GUN"));
	compGun->SetupAttachment(GetMesh(), TEXT("weapon_l"));
	compGun->SetRelativeLocation(FVector(0, 7, 5.5f));
}

void ANetPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Level 에 있는 모든 총을 찾자.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGun::StaticClass(), allGun);

	// Main UI 를 만들자.
	mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);
	mainUI->AddToViewport();

	// CameraBoom 초기 위치 설정
	CameraBoom->SetRelativeLocation(cameraBoomLocationWithoutGun);
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
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
    {
    	UE_LOG(LogTemp, Warning, TEXT("J 키 눌렀을 때"));
    }
	// 떼었을 때
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased(EKeys::J))
	{
		UE_LOG(LogTemp, Warning, TEXT("J 키 떼었을 때"));
	}
}

void ANetPlayer::TakeGun()
{
	if (hasGun == false)
	{
		// 현재까지 최단거리(초기값은 엄청 큰값 or 최대 범위 값)
		float closestDist = canTakeGunDist;
		// 현재까지 최단거리 총의 index
		int32 closestIdx = -1;
		// Level 에 있는 모든 총들 중에
		for (int32 i = 0; i < allGun.Num(); i++)
		{
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
			// 검색 된 총을 나에게 붙이자.
			AttachGun();
		}
	}
	// 총을 들고 있고, 재장전 중이 아니라면
	else if (isReload == false)
	{
		// ownedGun 을 담을 임시변수
		AGun* tempGun = ownedGun;
		// ownedGun 을 nullptr 로 설정
		ownedGun = nullptr;
		// 총을 버리자
		DetachGun(tempGun);
	}
}

void ANetPlayer::AttachGun()
{
	// 총을 집은 상태로 설정
	hasGun = true;
	// 물리적인 움직임 비활성
	UStaticMeshComponent* comp = ownedGun->GetComponentByClass<UStaticMeshComponent>();
	comp->SetSimulatePhysics(false);
	// compGun 에 총을 붙이자.
	ownedGun->AttachToComponent(compGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// 총 집었는지 여부에 따라 CameraBoom 설정 변경
	ChangeCameraBoomSetting();
	// 총알 UI를 총에 들어있는 총알 갯수만큼 채우자.
	mainUI->AddBullet(ownedGun->GetBulletCount());
	// Crosshair 활성
	mainUI->ShowCrosshair(true);
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
	// 총알 UI 모두 지우자
	mainUI->PopBulletAll();
	// Crosshair 비활성
	mainUI->ShowCrosshair(false);
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
	// 만약에 총을 들고있지 않으면 함수를 나가자.
	if (hasGun == false) return;
	// 총알이 없으면 함수를 나가자.
	if (ownedGun->GetBulletCount() <= 0) return;
	// 만약에 재장전 중이면 함수를 나가자.
	if (isReload) return;
	
	// 총 쏘는 애니메이션 실행
	PlayAnimMontage(playerMontage, 1.0f, FName(TEXT("Fire")));
	// 총알 갯수 하나 제거
	ownedGun->PopBullet();
	// 총알 UI 하나 제거
	mainUI->PopBullet();

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
	// 어딘가에 부딪혔다면
	if (bHit)
	{
		// UKismetMathLibrary::GetReflectionVector()
		// 입사각
		FVector inVector = endPos - startPos;
		// 법선벡터 (노멀벡터)
		FVector normalVector = hitInfo.Normal;
		// 반사각 계산
		float dot = FVector::DotProduct(inVector, normalVector);
		FVector outVector = inVector - 2 * dot * normalVector;
		// 반사각으로 파티클 효과가 재생되게 각도 구하자.
		FRotator rot = UKismetMathLibrary::MakeRotFromX(outVector);
		
		// 맞은 지점에 파티클 효과 표현
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect, hitInfo.Location, rot);
	}
}

void ANetPlayer::Reload()
{
	// 만약에 총을 들고있지 않으면 함수를 나가자.
	if (hasGun == false) return;
	// 총알이 가득 차 있다면 함수를 나가자.
	if (ownedGun->IsFillBullet()) return;
	// 만약에 재장전 중이면 함수를 나가자.
	if (isReload) return;

	// 재장전 중으로 설정
	isReload = true;	
	// 재장전 애니메이션 실행
	PlayAnimMontage(playerMontage, 1.0f, FName(TEXT("Reload")));
}

void ANetPlayer::OnReloadComplete()
{
	// 재장전 끝 설정
	isReload = false;
	// 총알 가득 채우자.
	ownedGun->FillBullet();
	// 총알 UI 가득 채우자
	mainUI->AddBullet(ownedGun->GetBulletCount());
}















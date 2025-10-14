// Fill out your copyright notice in the Description page of Project Settings.


#include "NetActor.h"

#include "NetPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANetActor::ANetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh Component 생성
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	SetRootComponent(compMesh);

	// 네트워크 동기화 대상으로 설정
	bReplicates = true;
}

// Called when the game starts or when spawned
void ANetActor::BeginPlay()
{
	Super::BeginPlay();

	// Dynamic Material 설정
	mat = compMesh->CreateDynamicMaterialInstance(0);

	// 1 초마다 색 변경하는 타이머
	FTimerHandle colorHandle;
	GetWorldTimerManager().SetTimer(colorHandle, this, &ANetActor::ChangeColor, 1.0f, true);
	
	// 1 초마다 크기 변경하는 타이머
	FTimerHandle scaleHandle;
	GetWorldTimerManager().SetTimer(scaleHandle, this, &ANetActor::ChangeScale, 1.0f, true);
}


void ANetActor::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate 할 변수들 등록
	DOREPLIFETIME(ANetActor, rotYaw);
	DOREPLIFETIME(ANetActor, matColor);
}

// Called every frame
void ANetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintNetLog();

	FindOwner();

	Rotate();
}

void ANetActor::PrintNetLog()
{
	// NetConnection 상태
	FString connStr = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");
	// Owner 상태
	FString ownerStr = GetOwner() != nullptr ? GetOwner()->GetActorNameOrLabel() : TEXT("Invalid Owner");
	
	// role
	// LOCAL : 내 입장에 보는 Role
	// REMOTE : [서버] 에서는 이 액터가 클라이언트에서는 *** 으로 Role 설정되어있다.
	//			[클라] 에서는 이 액터가 서버에서는 *** 으로 Role 설정되어있다
	FString roleStr = FString::Printf(TEXT("LOCAL : %s, REMOTE : %s"),
		*UEnum::GetValueAsString<ENetRole>(GetLocalRole()),
		*UEnum::GetValueAsString<ENetRole>(GetRemoteRole()));
	
	FString logStr = FString::Printf(TEXT("Connection : %s\r\nOwner : %s\r\nRole : %s"),
		*connStr, *ownerStr, *roleStr);

	DrawDebugString(GetWorld(), GetActorLocation(), logStr, nullptr, FColor::Yellow, 0, true);
}

void ANetActor::FindOwner()
{
	// 서버가 아니라면 함수를 나가자
	if (HasAuthority() == false) return;

	// 모든 NetPlayer 찾자.
	TArray<AActor*> allPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetPlayer::StaticClass(), allPlayer);

	// 현재까지 최단거리
	float closestDist = findDistance;
	// 현재까지 최단거리의 Player
	AActor* closestPlayer = nullptr;
	// allPlayer 에서 가장 가까운 Player 를 찾자.
	for (int32 i = 0; i < allPlayer.Num(); i++)
	{
		// i 번째 Player 와의 거리를 구하자.
		float dist = FVector::Distance(allPlayer[i]->GetActorLocation(), GetActorLocation());
		// 구한 거리가 Player 를 찾을 수 있는 범위 밖에 있으면 건너뛰자.
		if (dist > findDistance) continue;;
		// 그 거리가 현재까지 최단거리 보다 작으면
		if (dist < closestDist)
		{
			// 현재까지 최단거리 갱신
			closestDist = dist;
			// 현재까지 최단거리의 Player 갱신
			closestPlayer = allPlayer[i];
		}
	}

	// 만약에 현재 Onwer 와 최단거리의 Player 가 같다면 함수를 나가자.
	if (GetOwner() == closestPlayer) return;

	// closestPlayer 를 Owner 로 설정
	SetOwner(closestPlayer);
}

void ANetActor::Rotate()
{
	// 만약에 서버라면
	if (HasAuthority())
	{
		// yaw 축으로 회전시카자.
		AddActorWorldRotation(FRotator(0, 50 * GetWorld()->GetDeltaSeconds(), 0));
		// 현재 yaw 값을 rotYaw 에 설정
		rotYaw = GetActorRotation().Yaw;
	}
}

// ** 클라에서만 호출된다 ***
void ANetActor::OnRep_Rotate()
{
	// 현재 회전값을 가져오자
	FRotator rot = GetActorRotation();
	// 현재 회전값의 yaw 값을 서버에서 변경된 rotYaw 값으로 설정
	rot.Yaw = rotYaw;
	// 변경된 rot 값을 나의 회전값으로 설정
	SetActorRotation(rot);
}

void ANetActor::ChangeColor()
{
	// 서버가 아니면 함수 나가자.
	if (HasAuthority() == false) return;
	// 랜덤 색 설정
	matColor = FLinearColor::MakeRandomColor();
	// 서버에서는 직접 호출 하자!
	OnRep_ChangeColor();
}

void ANetActor::OnRep_ChangeColor()
{
	if (mat == nullptr) return;
	// mat 에 색 설정
	mat->SetVectorParameterValue(TEXT("FloorColor"), matColor);
}

void ANetActor::ChangeScale()
{
	
	// 만약에 Owner 가 나의 Player Pawn 으로 되어 있지 않으면 함수를 나가자.
	if (GetOwner() != GetWorld()->GetFirstPlayerController()->GetPawn()) return;
	
	// 서버에게 크기 바꿔주세요 요청
	ServerRPC_ChangeScale();
}



void ANetActor::ServerRPC_ChangeScale_Implementation()
{
	// 랜덤한 크기를 뽑자.
	float rand = FMath::RandRange(0.5f, 2.0f);

	// 클라에게 크기 바꿔!
	ClientRPC_ChangeScale(rand);
	// 모든 클라야 크기 바꿔라!
	//MulticastRPC_ChangeScale(rand);
}

void ANetActor::ClientRPC_ChangeScale_Implementation(float scale)
{
	// scale 값 설정
	SetActorScale3D(FVector(scale));
}

void ANetActor::MulticastRPC_ChangeScale_Implementation(float scale)
{
	// scale 값 설정
	SetActorScale3D(FVector(scale));
}

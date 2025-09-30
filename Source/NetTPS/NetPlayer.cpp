// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayer.h"

#include "EnhancedInputComponent.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"

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
}

void ANetPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 총 집기/ 놓기 Input 발생했을 때 호출되는 함수 등록
		enhancedInputComponent->BindAction(takeGunAction, ETriggerEvent::Started, this, &ANetPlayer::TakeGun);
	}
}

void ANetPlayer::TakeGun()
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
		UE_LOG(LogTemp, Warning, TEXT("최단 거리 총은 : %s"), *allGun[closestIdx]->GetActorNameOrLabel());
	}
}















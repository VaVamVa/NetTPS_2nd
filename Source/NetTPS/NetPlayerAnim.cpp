// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerAnim.h"

#include "NetPlayer.h"
#include "GameFramework/PawnMovementComponent.h"

void UNetPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 소유 Pawn 을 가져오자.
	pawnOwner = Cast<ANetPlayer>(TryGetPawnOwner());
}

void UNetPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (pawnOwner)
	{
		// 이동 속도 설정
		velocity = pawnOwner->GetVelocity();
		// 수평 이동 속력 설정
		groundSpeed = velocity.Size2D();
		// 공중 여부 설정
		isAir = pawnOwner->GetMovementComponent()->IsFalling();
		// 총 들고있는지 여부 설정
		hasGun = pawnOwner->hasGun;
		// 앞뒤 이동에 따른 값 설정
		FVector v = velocity;
		v.Z = 0;
		dirV = FVector::DotProduct(pawnOwner->GetActorForwardVector(), v);
		// 좌우 이동에 따른 값 설정
		dirH = FVector::DotProduct(pawnOwner->GetActorRightVector(), v);
		// 총을 들고 있을 때와 놓고 있을 때 pitchAngle, yawAngle 설정
		pitchAngle = hasGun ? -pawnOwner->GetBaseAimRotation().Pitch : 0;
		yawAngle = hasGun ? 20 : 0;
	}
}

void UNetPlayerAnim::AnimNotify_OnReloadComplete()
{
	if (pawnOwner)
	{
		// 소유 Pawn 의 재장전 완료 함수 호출
		pawnOwner->OnReloadComplete();
	}
}

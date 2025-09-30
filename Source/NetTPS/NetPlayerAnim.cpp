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
	}
}

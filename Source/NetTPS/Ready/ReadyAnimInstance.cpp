// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyAnimInstance.h"

#include "ReadyVisualCharacter.h"

void UReadyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if (AReadyVisualCharacter* castedActor = Cast<AReadyVisualCharacter>(GetOwningActor()))
	{
		ownerReadyActor = castedActor;
	}
}

void UReadyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (!IsValid(ownerReadyActor)) {return;}
	bReady = ownerReadyActor->IsReady();
}

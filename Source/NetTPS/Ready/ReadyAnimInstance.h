// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ReadyAnimInstance.generated.h"

class AReadyVisualCharacter;

/**
 * 
 */
UCLASS()
class NETTPS_API UReadyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Owner")
	TObjectPtr<AReadyVisualCharacter> ownerReadyActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Property)
	bool bReady = false;
};

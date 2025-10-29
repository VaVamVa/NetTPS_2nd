// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ReadyGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API AReadyGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void AddReadyCharacter(AActor* newCharacter);
	
protected:
	UPROPERTY()
	TArray<AActor*> ReadyCharacters;

	UPROPERTY(EditDefaultsOnly)
	FVector CenterLocation = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly)
	float Interval = 200.f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ReadyGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API AReadyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// ReadyPlayer 들
	UPROPERTY()
	TArray<AActor*> allReadyPlayer;

	void AddPlayer(AActor* player);
};

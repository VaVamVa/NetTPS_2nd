// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetTPSGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ANetTPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ANetTPSGameMode();
	
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawn>> characterList;
};




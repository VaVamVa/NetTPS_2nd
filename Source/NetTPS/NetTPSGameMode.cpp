// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetTPSGameMode.h"

#include "NetGameInstance.h"
#include "GameFramework/PlayerState.h"

ANetTPSGameMode::ANetTPSGameMode()
{
	// stub
}

APawn* ANetTPSGameMode::SpawnDefaultPawnAtTransform_Implementation(
	AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);

	// Player 가 선택한 Character 를 PawnClass 에 설정
	FString playerName = NewPlayer->PlayerState->GetPlayerName();
	int32 characterIdx = GetGameInstance<UNetGameInstance>()->GetSelectCharacter(playerName);
	if (characterIdx != -1)
	{
		PawnClass = characterList[characterIdx];
	}
	
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}
	return ResultPawn;
}



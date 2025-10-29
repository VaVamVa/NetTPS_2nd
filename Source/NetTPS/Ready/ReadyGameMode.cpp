// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyGameMode.h"

#include "ReadyGameState.h"
#include "ReadyPlayerState.h"
#include "ReadyPawn.h"

AReadyGameMode::AReadyGameMode()
{
	
}

void AReadyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
	AReadyPlayerState* ps = NewPlayer->GetPlayerState<AReadyPlayerState>();
	if (ps)
	{
		ps->ClientRPC_MakeReadyUI();
	}
	NewPlayer->SetInputMode(FInputModeUIOnly());
	NewPlayer->SetShowMouseCursor(true);
}

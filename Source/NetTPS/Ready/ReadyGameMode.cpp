// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyGameMode.h"

#include "ReadyPlayerState.h"

void AReadyGameMode::HandleStartingNewPlayer_Implementation(
	APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// 접속한 클라에게 Ready UI 만들어라!
	AReadyPlayerState* ps = NewPlayer->GetPlayerState<AReadyPlayerState>();
	if (ps)
	{
		ps->ClientRPC_MakeReadyUI();
	}
}

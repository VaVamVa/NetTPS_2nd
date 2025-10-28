// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyGameState.h"

void AReadyGameState::AddPlayer(AActor* player)
{
	// player 추가
	allReadyPlayer.Add(player);

	int32 playerCount = allReadyPlayer.Num();
	FVector startPos = FVector();
	startPos.Y = -(playerCount - 1) * 200 * 0.5f;
	for (int32 i = 0; i < playerCount; i++)
	{
		allReadyPlayer[i]->SetActorLocation(startPos + FVector::RightVector * 200 * i);
	}
}

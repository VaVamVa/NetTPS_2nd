// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"

void ANetPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
	onUpdatePlayerName.ExecuteIfBound(GetPlayerName());
}

void ANetPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	onUpdateScore.ExecuteIfBound(GetScore());
}

void ANetPlayerState::AddScore()
{
	Super::SetScore(Super::GetScore() + 1);
	if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone)
	{
		OnRep_Score();
	}
}

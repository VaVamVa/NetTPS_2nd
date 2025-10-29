// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"

#include "GameWidget.h"
#include "NetGameState.h"

void ANetPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(gameWidget))
	{
		gameWidget->AddToViewport();
	}
}

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

void ANetPlayerState::SetUI(UUserWidget* inUI)
{
}

void ANetPlayerState::ServerRPC_SendChat_Implementation(const FString& chat)
{
	NetMulticast_SendChat(chat);
}

void ANetPlayerState::NetMulticast_SendChat_Implementation(const FString& chat)
{
	GetWorld()->GetGameState<ANetGameState>()->gameWidget->UpdateChat(chat);
}
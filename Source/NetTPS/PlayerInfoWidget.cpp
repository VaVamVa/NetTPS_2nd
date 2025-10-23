// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UPlayerInfoWidget::UpdateName(const FString& newName)
{
	playerNameText->SetText(FText::FromString(newName));
}

void UPlayerInfoWidget::UpdateScore(const int32& newScore)
{
	playerScore->SetText(FText::AsNumber(newScore));
}

void UPlayerInfoWidget::SetPlayerId(const FUniqueNetIdRepl& playerUniqueNetID)
{
	if (uniqueNetId.IsValid()) {return;}
	uniqueNetId = playerUniqueNetID;
}

bool UPlayerInfoWidget::IsEqualPlayer(const FUniqueNetIdRepl& playerUniqueNetID)
{
	if (!uniqueNetId.IsValid()) {return false;}
	
	return uniqueNetId == playerUniqueNetID;
}

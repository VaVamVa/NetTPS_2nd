// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"

#include "NetPlayerState.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UPlayerInfoWidget::Init(ANetPlayerState* ps)
{
	ps->onUpdatePlayerName.BindUObject(this, &UPlayerInfoWidget::UpdateName);
	ps->onUpdateScore.BindUObject(this, &UPlayerInfoWidget::UpdateScore);
	if (!uniqueNetId.IsValid())
	{
		uniqueNetId = ps->GetUniqueId();
	}
}

void UPlayerInfoWidget::UpdateName(const FString& newName)
{
	playerNameText->SetText(FText::FromString(newName));
}

void UPlayerInfoWidget::UpdateScore(const int32& newScore)
{
	playerScore->SetText(FText::AsNumber(newScore));
}

bool UPlayerInfoWidget::IsEqualPlayer(const FUniqueNetIdRepl& playerUniqueNetID)
{
	if (!uniqueNetId.IsValid()) {return false;}
	
	return uniqueNetId == playerUniqueNetID;
}

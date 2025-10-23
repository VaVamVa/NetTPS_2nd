// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class NETTPS_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* playerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* playerScore;

	UFUNCTION()
	void UpdateName(const FString& newName);
	UFUNCTION()
	void UpdateScore(const int32& newScore);

	void SetPlayerId(const FUniqueNetIdRepl& playerId);
	bool IsEqualPlayer(const FUniqueNetIdRepl& playerUniqueNetID);

protected:
	FUniqueNetIdRepl uniqueNetId;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

class ANetPlayerState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class NETTPS_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(ANetPlayerState* ps);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* playerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* playerScore;
	
	bool IsEqualPlayer(const FUniqueNetIdRepl& playerUniqueNetID);

protected:
	UFUNCTION()
	void UpdateName(const FString& newName);
	UFUNCTION()
	void UpdateScore(const int32& newScore);

protected:
	FUniqueNetIdRepl uniqueNetId;
};

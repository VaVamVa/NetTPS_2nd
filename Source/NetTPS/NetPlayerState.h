// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetPlayerState.generated.h"

DECLARE_DELEGATE_OneParam(FOnUpdatePlayerName, const FString&)
DECLARE_DELEGATE_OneParam(FOnUpdateScore, const int32&)

class UPlayerInfoWidget;
/**
 * 
 */
UCLASS()
class NETTPS_API ANetPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void OnRep_PlayerName() override;
	virtual void OnRep_Score() override;

public:
	void AddScore();

	FOnUpdatePlayerName onUpdatePlayerName;
	FOnUpdateScore onUpdateScore;

protected:
	UPROPERTY()
	TObjectPtr<UPlayerInfoWidget> playerInfoWidget = nullptr;
};

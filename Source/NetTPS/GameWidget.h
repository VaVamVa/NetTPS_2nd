// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PlayerInfoList
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* playerInfoList;
	// PlayerInfoWidget 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerInfoWidget> playerInfoWidget;
	// PlayerInfoWidget 추가 함수
	void AddPlayerInfoUI(class ANetPlayerState* ps);
};

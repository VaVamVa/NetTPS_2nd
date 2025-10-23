// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 이름 Text
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* textName;
	// 점수 Text
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* textScore;
	// 이름 변경 함수
	UFUNCTION()
	void UpdateName(FString name);
	// 점수 변경 함수
	UFUNCTION()
	void UpdateScore(int32 score);
	// 초기화 함수
	void Init(class ANetPlayerState* ps);
};

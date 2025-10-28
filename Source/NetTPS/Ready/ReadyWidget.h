// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UReadyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// begin play
	virtual void NativeConstruct() override;

	// Player State
	UPROPERTY()
	class AReadyPlayerState* ps;
	
	// 버튼들 바인드
	UPROPERTY(meta=(BindWidget))
	class UButton* btnPlayer01;
	UPROPERTY(meta=(BindWidget))
	class UButton* btnPlayer02;
	UPROPERTY(meta=(BindWidget))
	class UButton* btnReady;
	UPROPERTY(meta=(BindWidget))
	class UButton* btnStart;
	// 버튼들 눌렸을 때 함수
	UFUNCTION()
	void OnClickPlayer01();
	UFUNCTION()
	void OnClickPlayer02();
	UFUNCTION()
	void OnClickReady();
	UFUNCTION()
	void OnClickStart();
	
};

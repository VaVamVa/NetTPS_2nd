// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin Play
	virtual void NativeConstruct() override;
	
	// Game Instance
	UPROPERTY()
	class UNetGameInstance* gi;
	// WigetSwitcher
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* widgetSwitcher;

	// 메인 화면 관련
	// 세션 생성 화면 이동 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* btnGoCreate;
	// 세션 조회 화면 이동 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* btnGoFind;
	// 세션 생성 화면 이동 함수
	UFUNCTION()
	void OnClickGoCreate();
	// 세션 조회 화면 이동 함수
	UFUNCTION()
	void OnClickGoFind();
};

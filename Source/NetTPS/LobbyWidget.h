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

	// 세션 생성 화면 관련
	// 세션 이름 입력
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* editSessionName;
	// 인원수 Slider
	UPROPERTY(meta=(BindWidget))
	class USlider* sliderPlayerCount;
	// 인원수 Text
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* textPlayerCount;
	// 생성 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* btnCreate;
	// 생성 버튼 눌렀을 때 호출되는 함수
	UFUNCTION()
	void OnClickCreate();
	// 세션 이름 변경 될 때 호출되는 함수
	UFUNCTION()
	void OnValueChangedSessionName(const FText& text);
	// 인원수 Slider 값이 변경 될 때 호출되는 함수
	UFUNCTION()
	void OnValueChangedPlayerCount(float value);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API USessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin Play
	virtual void NativeConstruct() override;
	// 세션 이름
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* textSessionName;
	// 참여 버튼
	UPROPERTY(meta=(BindWidget))
	class UButton* btnJoin;
	// 세션 목록 중 몇 번째 있는지
	int32 sessionIdx;
	// 정보 설정 함수
	void SetSessionInfo(int32 idx, FString sessionName);
	// 참여 버튼 클릭 함수
	UFUNCTION()
	void OnClickJoin();
};

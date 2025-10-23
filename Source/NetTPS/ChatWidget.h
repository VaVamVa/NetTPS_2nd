// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 채팅 내용
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* textContent;
	// 내용 변경 함수
	void SetContent(FString content);
};

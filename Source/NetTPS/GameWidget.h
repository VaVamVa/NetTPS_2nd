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
	// begin play
	virtual void NativeConstruct() override;
	
	// PlayerInfoList
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* playerInfoList;
	// PlayerInfoWidget 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerInfoWidget> playerInfoWidget;
	// PlayerInfoWidget 추가 함수
	void AddPlayerInfoUI(class ANetPlayerState* ps);

	// 채팅 내용 담고 있는 ScrollBox
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* scrollChat;
	// 채팅 내용 입력 하는 EditableTextBox
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* editChat;
	// 채팅 입력 후 엔터 쳤을때 호출되는 함수
	UFUNCTION()
	void OnTextBoxCommitted(const FText& text, ETextCommit::Type commitMethod);
	// ChatWidget 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChatWidget> chatWiget;
};

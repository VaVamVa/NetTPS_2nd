// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

class UBorder;
class UScrollBox;
class UChatWidget;
class UEditableTextBox;
class UVerticalBox;
class UPlayerInfoWidget;
class UButton;
/**
 * 
 */
UCLASS()
class NETTPS_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ChildWidget)
	TSubclassOf<UPlayerInfoWidget> playerInfoWidgetFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ChildWidget)
	TSubclassOf<UChatWidget> chatTextWidgetFactory;

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* playerInfoContainer;
	UPROPERTY(meta=(BindWidget))
	UScrollBox* chatScroll;
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* chatInputBox;
	
	void AddPlayerInfo(APlayerState* playerState);
	void RemovePlayerInfo(APlayerState* playerState);
	void AddScore(APlayerState* playerState);

	UFUNCTION()
	void OnChatInputTextCommited(const FText& inputText, ETextCommit::Type commitType);
	UFUNCTION()
	void UpdateChat(const FString& inStr);

	UPROPERTY(meta=(BindWidget))
	UBorder* inGameScreen;
	
	UFUNCTION()
	FEventReply OnPointerEvent(FGeometry myGeometry, const FPointerEvent& inPointerEvent);

	// BtnRetry 담을 변수
	UPROPERTY(meta=(BindWidget))
	class UButton* btnRetry;
	// BtnRetry 클릭했을 때 호출되는 함수
	UFUNCTION()
	void OnRetry();
	// BtnRetry 활성화 시켜주는 함수
	void ShowBtnRetry(const bool& bVisible);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

class UScrollBox;
class UChatWidget;
class UEditableTextBox;
class UVerticalBox;
class UPlayerInfoWidget;
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
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_UpdateChat(const FString& inStr);
};

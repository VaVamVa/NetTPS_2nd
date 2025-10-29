// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyWidget.generated.h"

class AReadyPlayerState;
class UButton;
/**
 * 
 */
UCLASS()
class NETTPS_API UReadyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta=(BindWidget))
	UButton* btnCharacter01;
	UPROPERTY(meta=(BindWidget))
	UButton* btnCharacter02;
	UPROPERTY(meta=(BindWidget))
	UButton* btnReady;
	UPROPERTY(meta=(BindWidget))
	UButton* btnPlay;

	UFUNCTION()
	void OnClickedCharacter01();
	UFUNCTION()
	void OnClickedCharacter02();
	UFUNCTION()
	void OnClickedReady();
	UFUNCTION()
	void OnClickedPlay();

	UFUNCTION()
	FORCEINLINE void SetPlayerState(AReadyPlayerState* newPS) {localPlayerState = newPS;}
	
private:
	UPROPERTY()
	TObjectPtr<AReadyPlayerState> localPlayerState;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ReadyPlayerState.generated.h"

class AReadyVisualCharacter;
class UReadyWidget;
/**
 * 
 */
UCLASS()
class NETTPS_API AReadyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_SelectCharacter(int32 characterIndex);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SelectCharacter(int32 characterIndex);
	UFUNCTION(Client, Reliable)
	void ClientRPC_MakeReadyUI();

	AReadyVisualCharacter* GetReadyVisualCharacter() {return readyVisualCharacter;};
	
protected:
	UPROPERTY()
	TObjectPtr<AReadyVisualCharacter> readyVisualCharacter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UReadyWidget> ReadyWidgetFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AReadyVisualCharacter> ReadyCharacterFactory;
	
};

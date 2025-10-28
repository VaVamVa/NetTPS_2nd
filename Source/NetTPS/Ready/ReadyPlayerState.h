// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ReadyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API AReadyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Ready Widget 만들어라
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UReadyWidget> readyWidget;
	UFUNCTION(Client, Reliable)
	void ClientRPC_MakeReadyUI();
	
	// 캐릭터 선택 관련 RPC, 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AReadyPlayer> readyPlayerFactory;
	UPROPERTY()
	class AReadyPlayer* readyPlayer;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_SelectPlayer(int32 playerIdx);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SelectPlayer(int32 playerIdx);

	// 래디 관련
	bool isReady;
	UFUNCTION(Server, Reliable)
	void ServerRPC_Ready();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Ready(bool ready);
};

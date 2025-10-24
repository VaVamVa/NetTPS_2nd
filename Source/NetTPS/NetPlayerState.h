// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetPlayerState.generated.h"

// 이름 변경 될 때 호출되는 Delegate
DECLARE_DELEGATE_OneParam(FUpdateName, FString);
// 점수 변경 될 때 호출되는 Delegate
DECLARE_DELEGATE_OneParam(FUpdateScore, int32);

UCLASS()
class NETTPS_API ANetPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// 이름 변경 될 때 호출되는 함수
	virtual void OnRep_PlayerName() override;
	// 점수 변경 될 때 호출되는 함수
	virtual void OnRep_Score() override;
	// 이름 Delgate
	FUpdateName onUpdateName;
	// 점수 Delgate
	FUpdateScore onUpdateScore;

	// 서버에게 채팅 내용 전달 RPC
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendChat(const FString& chat);
	// 모든 클라에게 채팅 내용 전달 RPC
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SendChat(const FString& chat);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyPlayerState.h"

#include "ReadyPlayer.h"
#include "ReadyWidget.h"

void AReadyPlayerState::ClientRPC_MakeReadyUI_Implementation()
{
	UReadyWidget* readyUI = CreateWidget<UReadyWidget>(GetWorld(), readyWidget);
	readyUI->ps = this;
	readyUI->AddToViewport();
}

void AReadyPlayerState::ServerRPC_SelectPlayer_Implementation(int32 playerIdx)
{
	// 클라이언트들에게 playerIdx 전달
	MulticastRPC_SelectPlayer(playerIdx);
}

void AReadyPlayerState::MulticastRPC_SelectPlayer_Implementation(
	int32 playerIdx)
{
	// ReadyPlayer 생성
	if (readyPlayer == nullptr)
	{
		readyPlayer = GetWorld()->SpawnActor<AReadyPlayer>(readyPlayerFactory);
	}
	// playerIdx 번째의 Mesh 로 설정
	readyPlayer->SetMesh(playerIdx);
}



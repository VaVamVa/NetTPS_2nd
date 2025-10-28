// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyPlayerState.h"

#include "ReadyGameState.h"
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
		// game state 에게 readyPlayer 추가하고 위치 조정
		AReadyGameState* gs = Cast<AReadyGameState>(GetWorld()->GetGameState());
		gs->AddPlayer(readyPlayer);
	}
	// playerIdx 번째의 Mesh 로 설정
	readyPlayer->SetMesh(playerIdx);
}


void AReadyPlayerState::ServerRPC_Ready_Implementation()
{
	// 모든 [클라] 에게 해당 Player 가 Ready 설정 / 해제 상태 보내자.
	MulticastRPC_Ready(!isReady);
}

void AReadyPlayerState::MulticastRPC_Ready_Implementation(bool ready)
{
	// 레디 상태 갱신
	isReady = ready;
	
	// 애니메이션 갱신
	readyPlayer->SetAnim(isReady ? 1 : 0);
}

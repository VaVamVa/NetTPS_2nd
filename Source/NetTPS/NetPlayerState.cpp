// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"

#include "GameWidget.h"
#include "NetGameState.h"

// 서버도 호출된다. 
void ANetPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	// 이름 변경된 것 알려주자 (PlayerInfoWidget 에게)
	onUpdateName.ExecuteIfBound(GetPlayerName());
}

void ANetPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	// 점수 변경된 것 알려주자 (PlayerInfoWidget 에게)
	onUpdateScore.ExecuteIfBound(GetScore());
}

void ANetPlayerState::ServerRPC_SendChat_Implementation(const FString& chat)
{
	// 채팅 내용을 ---> 이름 : 채팅내용
	FString sendChat = FString::Printf(TEXT("%s : %s"), *GetPlayerName(), *chat);
	// 모든 클라에게 sendChat 을 보내자.
	MulticastRPC_SendChat(sendChat);
}

void ANetPlayerState::MulticastRPC_SendChat_Implementation(const FString& chat)
{
	// GameState 가져오자.
	ANetGameState* gs = Cast<ANetGameState>(GetWorld()->GetGameState());
	// GameUI 의 AddChat 실행
	gs->gameUI->AddChat(chat);
}


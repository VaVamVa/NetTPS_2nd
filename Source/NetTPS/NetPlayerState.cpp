// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"

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

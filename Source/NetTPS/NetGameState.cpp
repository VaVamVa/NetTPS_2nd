// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameState.h"

#include "AnimationEditorTypes.h"
#include "NetPlayer.h"

void ANetGameState::AddPlayer(class ANetPlayer* player)
{
	// player 추가
	allPlayer.Add(player);
	// 만약에 추가된 Player 가 내것이라면 (이 뜻은 서버의 캐릭터라면)
	if (player->IsLocallyControlled())
	{
		// 최초 Turn 설정
		ChangeTurn();
	}
}

void ANetGameState::ChangeTurn()
{
	// 만약에 currTurnIdx 가 -1 이 아닐때
	if (currTurnIdx != -1)
	{
		// 이전 turn 인 Player 가 큐브를 만들지 못하게 하자.
		allPlayer[currTurnIdx]->canMakeCube = false;	
	}
	// turn 을 다음로 넘기자.
	currTurnIdx = (currTurnIdx + 1) % allPlayer.Num();
	// currTurnIdx 인 Player 가 큐브를 만들 수 있게 하자.
	allPlayer[currTurnIdx]->canMakeCube = true;
}

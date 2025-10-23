// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameState.h"

#include "AnimationEditorTypes.h"
#include "NetPlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.h"
#include "PlayerInfoWidget.h"
#include "GameFramework/PlayerState.h"

void ANetGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (!gameWidget)
	{
		gameWidget = CreateWidget<UGameWidget>(GetWorld(), gameWidgetFactory);
		gameWidget->AddToViewport();
	}
	gameWidget->AddPlayerInfo(PlayerState);
}

void ANetGameState::AddScore(APlayerState* PlayerState)
{
	if (IsValid(gameWidget))
	{
		gameWidget->AddScore(PlayerState);
	}
}

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

	// Player 위치 계산
	CalculatePos(player);
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

void ANetGameState::CalculatePos(class ANetPlayer* player)
{
	// 위치 갯수
	int32 posCnt = 3;
	// 360 도를 posCnt 갯수 만큼 분할한 각도
	float angle = 360.0f / posCnt;
	// angle 을 radian 값으로 변경
	float radian = FMath::DegreesToRadians(posIdx * angle);
	// 원점 기준에서 radian 각도로 회전 된 위치
	FVector dir = FVector(FMath::Sin(radian), FMath::Cos(radian), 0);
	// dir 방향으로 400 만큼 떨어진 위치
	FVector pos = player->GetActorLocation() + dir * 400;
	// Player 를 pos 위치로 이동
	player->SetActorLocation(pos);
	// posIdx 증가
	posIdx++;
}

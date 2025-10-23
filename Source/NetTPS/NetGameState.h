// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API ANetGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 모든 Player 를 담을 변수
	UPROPERTY()
	TArray<class ANetPlayer*> allPlayer;
	// 현재 큐브를 만들 수 있는 Player Index
	int32 currTurnIdx = -1;
	// Player 가 생성될 때 Player 를 추가하는 함수
	void AddPlayer(class ANetPlayer* player);
	// 큐브 만들 수 있는 Player 변경
	void ChangeTurn();

	// Player 생성 위치 Index
	int32 posIdx;
	// 위치 계산 함수
	void CalculatePos(class ANetPlayer* player);

	// PlayerState 추가될 때 호출되는 함수
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	// Game Widget 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameWidget> gameWidget;
	// Game Widget 결과물 담을 변수
	UPROPERTY()
	class UGameWidget* gameUI;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameState.generated.h"

class UPlayerInfoWidget;
class UGameWidget;
/**
 * 
 */
UCLASS()
class NETTPS_API ANetGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	// Game Widget Blueprint Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widget)
	TSubclassOf<UGameWidget> gameWidgetFactory;
	UPROPERTY()
	TObjectPtr<UGameWidget> gameWidget;
	void AddScore(APlayerState* PlayerState);
	
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

	void UpdateChatSystem(const FString& inputChatText);
};

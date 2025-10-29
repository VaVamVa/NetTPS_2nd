// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyWidget.h"

#include "ReadyGameState.h"
#include "ReadyPlayerState.h"
#include "Components/Button.h"

void UReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼들 클릭했을 때 호출되는 함수 등록
	btnPlayer01->OnClicked.AddDynamic(this, &UReadyWidget::OnClickPlayer01);
	btnPlayer02->OnClicked.AddDynamic(this, &UReadyWidget::OnClickPlayer02);
	btnReady->OnClicked.AddDynamic(this, &UReadyWidget::OnClickReady);
	btnStart->OnClicked.AddDynamic(this, &UReadyWidget::OnClickStart);
}

void UReadyWidget::OnClickPlayer01()
{
	ps->ServerRPC_SelectPlayer(0);
}

void UReadyWidget::OnClickPlayer02()
{
	ps->ServerRPC_SelectPlayer(1);
}

void UReadyWidget::OnClickReady()
{
	ps->ServerRPC_Ready();
}

void UReadyWidget::OnClickStart()
{
	if (ps->HasAuthority())
	{
		AReadyGameState* gs = Cast<AReadyGameState>(GetWorld()->GetGameState());
		if (gs->IsAllReady())
		{
			// 총 쏘는 맵 으로 이동
			GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Lvl_ThirdPerson?listen"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("모든 플레이어가 READY 상태가 아닙니다."))
		}
	}
}

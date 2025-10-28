// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyWidget.h"

#include "ReadyPlayerState.h"
#include "Components/Button.h"

void UReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 나의 PlayerState 가져오자.
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		ps = pc->GetPlayerState<AReadyPlayerState>();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player State is NULL"));
	}

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
}

void UReadyWidget::OnClickStart()
{
}

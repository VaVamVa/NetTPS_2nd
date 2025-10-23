// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "PlayerInfoWidget.h"
#include "Components/VerticalBox.h"

void UGameWidget::AddPlayerInfoUI(class ANetPlayerState* ps)
{
	// PlayerInfoWidget 만들자.
	UPlayerInfoWidget* info = CreateWidget<UPlayerInfoWidget>(GetWorld(), playerInfoWidget);	
	// PlayerInfoList 에 추가
	playerInfoList->AddChildToVerticalBox(info);
	// info 의 초기화 함수 실행
	info->Init(ps);
}

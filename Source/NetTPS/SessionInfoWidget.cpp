// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionInfoWidget.h"

#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 참여 버튼 클릭시 호출되는 함수 등록
	btnJoin->OnClicked.AddDynamic(this, &USessionInfoWidget::OnClickJoin);
}

void USessionInfoWidget::SetSessionInfo(int32 idx, FString sessionName)
{
	// 세션 순서 설정
	sessionIdx = idx;
	// 세션 이름 설정
	textSessionName->SetText(FText::FromString(sessionName));
}

void USessionInfoWidget::OnClickJoin()
{
	// Game Instance 가져오자
	UNetGameInstance* gi = Cast<UNetGameInstance>(GetGameInstance());
	// sessionIdx 번째 세션에 참여
	gi->JoinOtherSession(sessionIdx);
}

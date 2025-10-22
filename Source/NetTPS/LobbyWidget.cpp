// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "NetGameInstance.h"
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Game Instance 가져오자
	gi = Cast<UNetGameInstance>(GetGameInstance());
	// 세션 생성 버튼 클릭 함수 등록
	btnGoCreate->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGoCreate);
	// 세션 조회 버튼 클릭 함수 등록
	btnGoFind->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGoFind);
}

void ULobbyWidget::OnClickGoCreate()
{
}

void ULobbyWidget::OnClickGoFind()
{
}

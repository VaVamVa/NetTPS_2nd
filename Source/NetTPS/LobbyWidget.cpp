// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "SessionInfoWidget.h"
#include "Components/ScrollBox.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 마우스 커서 보이게
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	// Game Instance 가져오자
	gi = Cast<UNetGameInstance>(GetGameInstance());
	// 세션 생성 버튼 클릭 함수 등록
	btnGoCreate->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGoCreate);
	// 세션 조회 버튼 클릭 함수 등록
	btnGoFind->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickGoFind);

	// 생성 버튼 클릭 함수 등록
	btnCreate->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickCreate);
	// 세션 이름 변경 될 때 호출되는 함수 등록
	editSessionName->OnTextChanged.AddDynamic(this, &ULobbyWidget::OnValueChangedSessionName);
	// 인원 수 변경 될 때 호출되는 함수 등록
	sliderPlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnValueChangedPlayerCount);

	// 세션 조회 버튼 클릭 함수 등록
	btnFind->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickFind);
	// GameInstace 의 delegate 등록
	gi->onFindComplete.BindUObject(this, &ULobbyWidget::OnFindComplete);
}

void ULobbyWidget::OnClickGoCreate()
{
	// 세션 생성 화면으로 이동
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnClickGoFind()
{
	// 세션 조회 화면으로 이동
	widgetSwitcher->SetActiveWidgetIndex(2);
}

void ULobbyWidget::OnClickCreate()
{
	// 세션 이름
	FString sessionName = editSessionName->GetText().ToString();
	// 인원 수
	int32 playerCount = sliderPlayerCount->GetValue();
	// 세션 생성
	gi->CreateMySession(sessionName, playerCount);
}

void ULobbyWidget::OnValueChangedSessionName(const FText& text)
{
	// text 값의 길이에 따른 생성 버튼 활성 / 비활성
	// text.IsEmpty()
	btnCreate->SetIsEnabled(text.ToString().Len() > 0);
}

void ULobbyWidget::OnValueChangedPlayerCount(float value)
{
	// value 값을 textPlayerCount 에 설정
	textPlayerCount->SetText(FText::AsNumber(value));
}

void ULobbyWidget::OnClickFind()
{
	// 세션 조회
	gi->FindOtherSession();
}

void ULobbyWidget::OnFindComplete(int32 idx, FString sessionName)
{
	// sessionInfoWidget 만들자.
	USessionInfoWidget* item = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidget);
	// 만들어진 item 을 scrollSessionList 에 추가
	scrollSessionList->AddChild(item);
	// 만들어지 item 정보 설정
	item->SetSessionInfo(idx, sessionName);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

#include "ChatWidget.h"
#include "PlayerInfoWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 채팅 입력 후 엔터쳤을 때 호출되는 함수 등록
	editChat->OnTextCommitted.AddDynamic(this, &UGameWidget::UGameWidget::OnTextBoxCommitted);
}

void UGameWidget::AddPlayerInfoUI(class ANetPlayerState* ps)
{
	// PlayerInfoWidget 만들자.
	UPlayerInfoWidget* info = CreateWidget<UPlayerInfoWidget>(GetWorld(), playerInfoWidget);	
	// PlayerInfoList 에 추가
	playerInfoList->AddChildToVerticalBox(info);
	// info 의 초기화 함수 실행
	info->Init(ps);
}

void UGameWidget::OnTextBoxCommitted(const FText& text,
	ETextCommit::Type commitMethod)
{
	// 만약에 엔터를 친 이벤트면
	if (commitMethod == ETextCommit::OnEnter)
	{
		UE_LOG(LogTemp, Warning, TEXT("content : %s"), *text.ToString());
		// 채팅 UI 만들어서 채팅 내용에 추가
		UChatWidget* chat = CreateWidget<UChatWidget>(GetWorld(), chatWiget);
		scrollChat->AddChild(chat);
		chat->SetContent(text.ToString());
		// editChat 에 남아있는 내용 초기화
		editChat->SetText(FText());
	}
	else if (commitMethod == ETextCommit::OnCleared)
	{
		// 강제로 editChat 을 활성화
		editChat->SetFocus();
	}
}

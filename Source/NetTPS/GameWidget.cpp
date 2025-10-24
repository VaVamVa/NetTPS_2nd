// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

#include "ChatWidget.h"
#include "NetPlayerState.h"
#include "NetTPSPlayerController.h"
#include "PlayerInfoWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 채팅 입력 후 엔터쳤을 때 호출되는 함수 등록
	editChat->OnTextCommitted.AddDynamic(this, &UGameWidget::OnTextBoxCommitted);
	// Border 마우스 클릭시 호출되는 함수 등록
	borderEmpty->OnMouseButtonDownEvent.BindDynamic(this, &UGameWidget::OnPointerEvent);

	// BtnRetry 클릭했을 때 호출되는 함수 등록
	btnRetry->OnClicked.AddDynamic(this, &UGameWidget::OnRetry);
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
		// 서버에게 채팅 내용 전달
		// 내 PlayerState 가져오자.
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		ANetPlayerState* ps = pc->GetPlayerState<ANetPlayerState>();
		ps->ServerRPC_SendChat(text.ToString());

		// editChat 에 남아있는 내용 초기화
		editChat->SetText(FText());
	}
	else if (commitMethod == ETextCommit::OnCleared)
	{
		// 강제로 editChat 을 활성화
		editChat->SetFocus();
	}
}

void UGameWidget::AddChat(FString text)
{
	// 현재 스크롤 위치 값
	float scrollOffset = scrollChat->GetScrollOffset();
	// 스크롤이 맨 끝일때 값
	float scrollOffsetOfEnd = scrollChat->GetScrollOffsetOfEnd();
	
	// 채팅 UI 만들어서 채팅 내용에 추가
	UChatWidget* chat = CreateWidget<UChatWidget>(GetWorld(), chatWiget);
	scrollChat->AddChild(chat);
	chat->SetContent(text);
	

	// 만약에 스크롤이 위치가 맨 끝이라면
	if (scrollOffset == scrollOffsetOfEnd)
	{
		// 개행되는 채팅이 추가되면 한줄로 크기를 인식해서 발생하는 문제 때문에
		// ScrollToEnd 를 0.01 초 뒤에 실행
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			// 스크롤 위치를 맨 끝으로 해라!
			scrollChat->ScrollToEnd();
		}, 0.01f, false);
	}
}

FEventReply UGameWidget::OnPointerEvent(FGeometry MyGeometry,
	const FPointerEvent& MouseEvent)
{
	// Input Mode 를 GameOnly 로 설정
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
	// 마우스 포인터 비활성화
	pc->SetShowMouseCursor(false);

	return UWidgetBlueprintLibrary::Handled();
}

void UGameWidget::OnRetry()
{
	// 관전자로 전환
	ANetTPSPlayerController* pc = Cast<ANetTPSPlayerController>(GetWorld()->GetFirstPlayerController());
	// [서버] 에게 관전자로 변경 요청
	pc->ServerRPC_ChangeToSpectator();
	// 마우스 보이지 않게
	pc->SetShowMouseCursor(false);
	// 다시하기 버튼 보이지 않게
	ShowBtnRetry(false);	
}

void UGameWidget::ShowBtnRetry(bool visible)
{
	// BtnRetry 보이게
	btnRetry->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

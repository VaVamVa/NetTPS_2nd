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
#include "GameFramework/PlayerState.h"

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	chatInputBox->OnTextCommitted.AddDynamic(this, &UGameWidget::OnChatInputTextCommited);
	chatInputBox->SetClearKeyboardFocusOnCommit(true);

	inGameScreen->OnMouseButtonDownEvent.BindDynamic(this, &UGameWidget::OnPointerEvent);

	// BtnRetry 가져오자.
	//btnRetry = Cast<UButton>(GetWidgetFromName(TEXT("BtnRetry")));
	// BtnRetry 클릭했을 때 호출되는 함수 등록
	btnRetry->OnClicked.AddDynamic(this, &UGameWidget::OnRetry);
}

void UGameWidget::AddPlayerInfo(APlayerState* playerState)
{
	UPlayerInfoWidget* infoWidget = CreateWidget<UPlayerInfoWidget>(playerInfoContainer, playerInfoWidgetFactory);
	infoWidget->Init(Cast<ANetPlayerState>(playerState));
	playerInfoContainer->AddChildToVerticalBox(infoWidget);
}

void UGameWidget::RemovePlayerInfo(APlayerState* playerState)
{
	for (UWidget* child : playerInfoContainer->GetAllChildren())
	{
		if (UPlayerInfoWidget* infoWidget = Cast<UPlayerInfoWidget>(child);
			infoWidget->IsEqualPlayer(playerState->GetUniqueId())
		)
		{
			playerInfoContainer->RemoveChild(infoWidget);
			return;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("This Player Not Exist in Game Widget!"));
}

void UGameWidget::AddScore(APlayerState* playerState)
{
	for (UWidget* child : playerInfoContainer->GetAllChildren())
	{
		if (UPlayerInfoWidget* infoWidget = Cast<UPlayerInfoWidget>(child);
			infoWidget->IsEqualPlayer(playerState->GetUniqueId())
		)
		{
			playerState->SetScore(playerState->GetScore() + 1);
			if (playerState->GetNetMode() == NM_ListenServer || playerState->GetNetMode() == NM_Standalone)
			{
				playerState->OnRep_Score();
			}
			return;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("This Player Not Exist in Game Widget!"));
}


void UGameWidget::OnChatInputTextCommited(const FText& inputText, ETextCommit::Type commitType)
{
	if (commitType == ETextCommit::Type::OnEnter && !inputText.IsEmptyOrWhitespace())
	{
		const FString chat = GetOwningPlayerState()->GetPlayerName() + " : " + inputText.ToString();
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		ANetPlayerState* ps = pc->GetPlayerState<ANetPlayerState>();
		ps->ServerRPC_SendChat(chat);
		
		chatInputBox->SetText(FText());
	}
	FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}

void UGameWidget::UpdateChat(const FString& inStr)
{
	UChatWidget* chatLine = CreateWidget<UChatWidget>(chatScroll, chatTextWidgetFactory);
	float scrollOffset = chatScroll->GetScrollOffset();
	float scrollOffsetOfEnd = chatScroll->GetScrollOffsetOfEnd();
	
	chatLine->SetChatText(FText::FromString(inStr));
	chatScroll->AddChild(chatLine);
	
	if (scrollOffset == scrollOffsetOfEnd)
	{
		// 개행 채팅 인식 interval 기다리기
		GetWorld()->GetTimerManager().SetTimerForNextTick(
			[this]() -> void
			{
				chatScroll->ScrollToEnd();
			}
		);
		
	}
}

FEventReply UGameWidget::OnPointerEvent(FGeometry myGeometry, const FPointerEvent& inPointerEvent)
{
	return {};
}

void UGameWidget::OnRetry()
{
	// 관전자로 전환
	ANetTPSPlayerController* pc = Cast<ANetTPSPlayerController>(GetWorld()->GetFirstPlayerController());
	// [서버] 에게 관전자로 변경 요청
	pc->ServerRPC_ChangeToSpectator();
	// 마우스 보이지 않게
	pc->SetShowMouseCursor(false);
	// 다시하기 버튼 안보이게
	ShowBtnRetry(false);
}

void UGameWidget::ShowBtnRetry(const bool& bVisible)
{
	// BtnRetry 보이게
	btnRetry->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}



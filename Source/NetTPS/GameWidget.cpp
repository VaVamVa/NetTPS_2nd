// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

#include "ChatWidget.h"
#include "PlayerInfoWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	chatInputBox->OnTextCommitted.AddDynamic(this, &UGameWidget::UGameWidget::OnChatInputTextCommited);
	chatInputBox->SetClearKeyboardFocusOnCommit(true);
}

void UGameWidget::AddPlayerInfo(APlayerState* playerState)
{
	UPlayerInfoWidget* infoWidget = CreateWidget<UPlayerInfoWidget>(playerInfoContainer, playerInfoWidgetFactory);
	infoWidget->SetPlayerId(playerState->GetUniqueId());
	infoWidget->UpdateName(playerState->GetName());
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
			infoWidget->UpdateScore(playerState->GetScore());
			return;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("This Player Not Exist in Game Widget!"));
}


void UGameWidget::OnChatInputTextCommited(const FText& inputText, ETextCommit::Type commitType)
{
	if (commitType == ETextCommit::Type::OnEnter && !inputText.IsEmpty())
	{
		const FString chat = GetOwningPlayerState()->GetPlayerName() + " : " + inputText.ToString();
		
		NetMulticastRPC_UpdateChat(chat);
		chatInputBox->SetText(FText());
	}
	FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}

void UGameWidget::NetMulticastRPC_UpdateChat_Implementation(const FString& inStr)
{
	UChatWidget* chatLine = CreateWidget<UChatWidget>(chatScroll, chatTextWidgetFactory);
	chatLine->SetChatText(FText::FromString(inStr));
	chatScroll->ScrollToEnd();
	chatScroll->AddChild(chatLine);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyWidget.h"

#include "ReadyVisualCharacter.h"
#include "Ready/ReadyPlayerState.h"
#include "Components/Button.h"

void UReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btnCharacter01->OnClicked.AddDynamic(this, &UReadyWidget::OnClickedCharacter01);
	btnCharacter02->OnClicked.AddDynamic(this, &UReadyWidget::OnClickedCharacter02);
	btnReady->OnClicked.AddDynamic(this, &UReadyWidget::OnClickedReady);
	btnPlay->OnClicked.AddDynamic(this, &UReadyWidget::OnClickedPlay);
}

void UReadyWidget::OnClickedCharacter01()
{
	localPlayerState->ServerRPC_SelectCharacter(0);
}

void UReadyWidget::OnClickedCharacter02()
{
	localPlayerState->ServerRPC_SelectCharacter(1);
}

void UReadyWidget::OnClickedReady()
{
	localPlayerState->GetReadyVisualCharacter()->SetMeshReadyState(false);
}

void UReadyWidget::OnClickedPlay()
{
}

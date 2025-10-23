// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"

#include "NetPlayerState.h"
#include "Components/TextBlock.h"

void UPlayerInfoWidget::UpdateName(FString name)
{
	textName->SetText(FText::FromString(name));
}

void UPlayerInfoWidget::UpdateScore(int32 score)
{
	textScore->SetText(FText::AsNumber(score));
}

void UPlayerInfoWidget::Init(class ANetPlayerState* ps)
{
	// ps 에 UpdateName 함수 등록
	ps->onUpdateName.BindUObject(this, &UPlayerInfoWidget::UpdateName);
	// ps 에 UpdateScore 함수 등록
	ps->onUpdateScore.BindUObject(this, &UPlayerInfoWidget::UpdateScore);
}

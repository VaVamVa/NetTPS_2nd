// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyPlayerState.h"

#include "ReadyGameState.h"
#include "ReadyVisualCharacter.h"

#include "ReadyWidget.h"

void AReadyPlayerState::ServerRPC_SelectCharacter_Implementation(int32 characterIndex)
{
	NetMulticast_SelectCharacter(characterIndex);
}

void AReadyPlayerState::NetMulticast_SelectCharacter_Implementation(int32 characterIndex)
{
	if (readyVisualCharacter == nullptr)
	{
		readyVisualCharacter = GetWorld()->SpawnActor<AReadyVisualCharacter>(ReadyCharacterFactory);
		GetWorld()->GetGameState<AReadyGameState>()->AddReadyCharacter(readyVisualCharacter);
	}

	readyVisualCharacter->SetMesh(characterIndex);
}

void AReadyPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AReadyPlayerState::ClientRPC_MakeReadyUI_Implementation()
{
	UReadyWidget* widget = CreateWidget<UReadyWidget>(GetPlayerController(), ReadyWidgetFactory, TEXT("ReadyWidget"));
	widget->SetPlayerState(this);
	widget->AddToViewport();
}

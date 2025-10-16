// Copyright Epic Games, Inc. All Rights Reserved.


#include "NetTPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "NetTPS.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Widgets/Input/SVirtualJoystick.h"

void ANetTPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogNetTPS, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ANetTPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void ANetTPSPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	// 현재 Possess 하고 있는 Pawn 을 가져오자.
	APawn* pawn = GetPawn();
	// 현재 GameMode 가져오자.
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	// Unpossess 하자.
	UnPossess();
	// 관전자 Pawn 생성
	ASpectatorPawn* spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, pawn->GetTransform());
	// 생성된 관전자로 Possess 하자.
	Possess(spectator);
	// 기존에 있던 Pawn(Player) 를 제거
	pawn->Destroy();
}

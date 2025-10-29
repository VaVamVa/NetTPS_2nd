// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyPawn.h"


// Sets default values
AReadyPawn::AReadyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AReadyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AReadyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	FInputModeUIOnly inputModeUIOnly;
	GetController<APlayerController>()->SetInputMode(inputModeUIOnly);
}


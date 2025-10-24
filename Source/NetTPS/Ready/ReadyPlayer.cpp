// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyPlayer.h"


// Sets default values
AReadyPlayer::AReadyPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReadyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReadyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


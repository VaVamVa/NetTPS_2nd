// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyGameState.h"


void AReadyGameState::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void AReadyGameState::AddReadyCharacter(AActor* newCharacter)
{
	ReadyCharacters.Add(newCharacter);
	FVector StartLocation = CenterLocation;
	StartLocation.Y = -(ReadyCharacters.Num() - 1) * Interval * 0.5f;

	float intervalIter = 0.f;
	for (int32 i = 0; i < ReadyCharacters.Num(); ++i)
	{
		ReadyCharacters[i]->SetActorLocation(StartLocation + FVector(0.f, intervalIter, 0.f));
		intervalIter += Interval;
	}
}

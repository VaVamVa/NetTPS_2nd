// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyPlayer.h"


// Sets default values
AReadyPlayer::AReadyPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	SetRootComponent(mesh);
}

// Called when the game starts or when spawned
void AReadyPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(0, 90, 0));
}

// Called every frame
void AReadyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AReadyPlayer::SetMesh(int32 idx)
{
	// idx 번째의 Mesh 설정
	mesh->SetSkeletalMesh(playerMeshArray[idx]);
}


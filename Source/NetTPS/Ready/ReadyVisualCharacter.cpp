// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyVisualCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "ReadyAnimInstance.h"


// Sets default values
AReadyVisualCharacter::AReadyVisualCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(meshComponent);
	meshComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	meshComponent->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
}

void AReadyVisualCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

// Called when the game starts or when spawned
void AReadyVisualCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void AReadyVisualCharacter::SetMesh(int32 meshIndex)
{
	if (!availableMeshArray.IsValidIndex(meshIndex)) {return;}
	currentMeshIndex = meshIndex;
	meshComponent->SetSkeletalMesh(availableMeshArray[meshIndex].MeshAsset);
	meshComponent->PlayAnimation(availableMeshArray[meshIndex].DefaultAnimationAsset, true);
}

void AReadyVisualCharacter::SetMeshReadyState(const bool& inReadyState)
{
	meshComponent->SetAnimation(availableMeshArray[currentMeshIndex].GetAnimFromState(inReadyState));
}




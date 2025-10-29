// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReadyVisualCharacter.generated.h"

class USkeletalMeshComponent;
class UReadyAnimInstance;

USTRUCT(BlueprintType)
struct NETTPS_API FReadyMesh
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMesh> MeshAsset;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> DefaultAnimationAsset;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> ReadyAnimationAsset;

	FORCEINLINE UAnimSequence* GetAnimFromState(const bool& readyState)
	{
		return readyState ? ReadyAnimationAsset : DefaultAnimationAsset;
	}
};

UCLASS()
class NETTPS_API AReadyVisualCharacter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReadyVisualCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	bool IsReady() const { return bReady; };

	UFUNCTION()
	void SetMesh(int32 meshIndex);
	UFUNCTION()
	void SetMeshReadyState(const bool& inReadyState);

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() {return meshComponent;}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	bool bReady = false;

	int32 currentMeshIndex = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<USkeletalMeshComponent> meshComponent;
	UPROPERTY(EditDefaultsOnly)
	TArray<FReadyMesh> availableMeshArray;

	

};

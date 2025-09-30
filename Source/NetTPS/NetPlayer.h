// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetTPSCharacter.h"
#include "NetPlayer.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API ANetPlayer : public ANetTPSCharacter
{
	GENERATED_BODY()

public:
	ANetPlayer();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
public:
	// 총 집기, 놓기 InputAction
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* takeGunAction;
	// 총을 붙일 Component
	UPROPERTY(EditAnywhere)
	class USceneComponent* compGun;
	// Level 에 있는 모든 총을 담을 변수
	UPROPERTY(EditAnywhere)
	TArray<class AActor*> allGun;
	// 총을 잡을 수 있는 범위
	UPROPERTY(EditAnywhere)
	float canTakeGunDist = 200;

	// 총 집기 / 놓기 함수
	void TakeGun();
};

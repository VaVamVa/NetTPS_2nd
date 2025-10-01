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

	// 총을 집고 있는지 여부
	bool hasGun;
	// 집은 총 담을 변수
	UPROPERTY()
	class AGun* ownedGun;

	// 총 집기 / 놓기 함수
	void TakeGun();
	// 총을 붙이는 함수
	void AttachGun();
	// 총을 버리는 함수
	void DetachGun(class AGun* gun);
};

















// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class NETTPS_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 최대 총알 갯수
	int32 maxBulletCnt = 10;
	// 현재 총알 갯수
	int32 currBulletCnt = maxBulletCnt;

	// 현재 총알 갯수 반환 함수
	int32 GetBulletCount() { return currBulletCnt; }
	// 현재 총알 갯수 줄이는 함수
	void PopBullet() { currBulletCnt--; }
	// 총알 가득 채우기 함수
	void FillBullet() { currBulletCnt = maxBulletCnt; }
	// 총알이 가득 찼는지 여부
	bool IsFillBullet() { return currBulletCnt == maxBulletCnt; }
};




















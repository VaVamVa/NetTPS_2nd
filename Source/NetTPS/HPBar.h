// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBar.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	// ProgressBar 담을 변수
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* hpBar;
	// 최대 HP
	float maxHP = 100;
	// 현재 HP
	float currHP = maxHP;
	// 현재 HP 갱신 함수
	void UpdateHP(float damage);
};

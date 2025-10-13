// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// WBP_Bullet 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> bulletWidget;
	// HorizontalBox - MagazineBox 담을 변수
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* magazineBox;

	// 총알 UI 를 현재 총알 갯수 만큼 추가 함수
	void AddBullet(int32 bulletCnt);
	// 총알 UI 하나씩 제거 함수
	void PopBullet();
	// 총알 UI 전체 제거 함수
	void PopBulletAll();

	// crosshair 이미지 담을 변수
	UPROPERTY(meta=(BindWidget))
	class UImage* crosshair;
	// crosshair 활성/비활성 하는 함수
	void ShowCrosshair(bool show);

	// HPBarUI 담을 변수
	UPROPERTY(meta=(BindWidget))
	class UHPBar* hpBarUI;

	// DamageUI 담을 변수
	UPROPERTY(meta=(BindWidget))
	class UImage* damageUI;
	// DamageUI 의 Opacity 값
	float damageUIOpacity;
	// DamageUI 보이게 하는 함수
	void ShowDamageUI();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

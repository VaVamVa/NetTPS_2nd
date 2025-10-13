// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"

#include "Components/ProgressBar.h"

float UHPBar::UpdateHP(float damage)
{
	// 현재 HP 를 damage 만큼 줄이자.
	currHP -= damage;
	// 현재 HP 를 0 ~ 1 사이 값으로 만들자.
	float percent = currHP / maxHP;
	// 계산한 값을 hpBar 에 설정
	hpBar->SetPercent(percent);

	// 현재 HP 을 반환
	return currHP;
}

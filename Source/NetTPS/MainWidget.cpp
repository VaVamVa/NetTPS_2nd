// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "NetTPSCharacter.h"
#include "NetTPSPlayerController.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UMainWidget::AddBullet(int32 bulletCnt)
{
	// 전체 총알 UI 제거
	PopBulletAll();
	
	// bulletCnt 만큼 총알 UI 생성해서
	// MagazineBox 의 자식으로 설정
	for (int32 i = 0; i < bulletCnt; i++)
	{
		UUserWidget* bulletUI = CreateWidget(GetWorld(), bulletWidget);
		magazineBox->AddChildToHorizontalBox(bulletUI);
	}
}

void UMainWidget::PopBullet()
{
	// 첫 번째 총알 위젯을 지우자.
	magazineBox->RemoveChildAt(0);
}

void UMainWidget::PopBulletAll()
{
	// 모든 총알을 지우자.
	magazineBox->ClearChildren();
}

void UMainWidget::ShowCrosshair(bool show)
{
	// show 값에 따라서 crosshair 활성 / 비활성
	crosshair->SetVisibility(show ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainWidget::ShowDamageUI()
{
	damageUIOpacity = 1;
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 만약에 damageUIOpacity 값이 0 보다 크면
	if (damageUIOpacity > 0)
	{
		// damageUIOpacity 값을 줄이자.
		damageUIOpacity -= InDeltaTime * 3;
		// 0 보다 작게 만들지 말자.
		damageUIOpacity = FMath::Max(damageUIOpacity, 0);
		// damageUIOpacity 으로 damageUI 의 RenderOpacity 값 설정
		damageUI->SetRenderOpacity(damageUIOpacity);
	}
}



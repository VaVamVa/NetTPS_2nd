
#include "BossStateBase.h"

#include "Boss.h"

BossStateBase::BossStateBase(class ABoss* ownedPawn)
	: boss(ownedPawn), delayTime(0), currTime(0) 
{
}

void BossStateBase::OnEnter()
{
}

void BossStateBase::OnUpdate(float deltaTime)
{
}

void BossStateBase::OnExit()
{
}

bool BossStateBase::IsDelayComplete()
{
	// 시간 누적
	currTime += boss->GetWorld()->GetDeltaSeconds();
	// delay 시간 만큼 기다렸다면
	if (currTime > delayTime)
	{
		// 현재 시간 초기화
		currTime = 0;
		// true 반환
		return true;
	}
	return false;
}

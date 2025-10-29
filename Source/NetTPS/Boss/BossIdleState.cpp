#include "BossIdleState.h"

#include "Boss.h"

BossIdleState::BossIdleState(class ABoss* ownedPawn)
	: BossStateBase(ownedPawn)
{
}

void BossIdleState::OnEnter()
{
	BossStateBase::OnEnter();
	// 대기 시간 (2 ~ 4초) 사이 값으로 설정
	delayTime = FMath::RandRange(2.0f, 4.0f);
}

void BossIdleState::OnUpdate(float deltaTime)
{
	BossStateBase::OnUpdate(deltaTime);

	// 만약에 delayTime 만큼 기다렸다면
	if (IsDelayComplete())
	{
		// 상태를 점프 공격 상태로 전환
		boss->ChangeState(EBossState::ATTACK_JUMP);
	}
}

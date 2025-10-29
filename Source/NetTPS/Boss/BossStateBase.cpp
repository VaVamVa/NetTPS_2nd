
#include "BossStateBase.h"

BossStateBase::BossStateBase(class ABoss* ownedPawn)
	: boss(ownedPawn)
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

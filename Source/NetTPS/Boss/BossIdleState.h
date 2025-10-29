#pragma once
#include "BossStateBase.h"

class BossIdleState : public BossStateBase
{
public:
	BossIdleState(class ABoss* ownedPawn);
	virtual ~BossIdleState() {}

	virtual void OnEnter() override;
	virtual void OnUpdate(float deltaTime) override;
};

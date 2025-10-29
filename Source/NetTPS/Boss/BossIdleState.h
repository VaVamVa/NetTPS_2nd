#pragma once
#include "BossStateBase.h"

class BossIdleState : public BossStateBase
{
public:
	BossIdleState(class ABoss* ownedPawn);
};

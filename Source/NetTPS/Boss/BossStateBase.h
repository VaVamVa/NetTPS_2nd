#pragma once

class BossStateBase
{
public:
	BossStateBase(class ABoss* ownedPawn);
	
	// 일반 클래스에서는 boss 변수가 GC 대상이 아님
	class ABoss* boss;

	virtual void OnEnter();
	virtual void OnUpdate(float deltaTime);
	virtual void OnExit();

	// 대기 시간
	float delayTime;
	// 현재 시간
	float currTime;
	// delayTime 체크 함수
	bool IsDelayComplete();

};

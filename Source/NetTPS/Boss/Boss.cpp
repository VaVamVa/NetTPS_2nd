// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"

#include "BossAttackJumpState.h"
#include "BossIdleState.h"


// Sets default values
ABoss::ABoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	// 상태 클래스들 추가
	stateList.Add(MakeShared<BossIdleState>(this));
	stateList.Add(MakeShared<BossAttackJumpState>(this));

	// 초기 상태를 IDLE 로
	ChangeState(EBossState::IDLE);
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (currStateClass != nullptr)
	{
		// 현재 상태 OnUpdate 함수 호출
		currStateClass->OnUpdate(DeltaTime);
	}
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoss::ChangeState(EBossState newState)
{
	// 이전 상태 OnExit 함수 호출
	currStateClass->OnExit();
	// 현재 상태를 newState 상태로 설정
	currState = newState;
	currStateClass = stateList[(int)newState];
	// 현재 상태 OnEnter 함수 호출
	currStateClass->OnEnter();
}


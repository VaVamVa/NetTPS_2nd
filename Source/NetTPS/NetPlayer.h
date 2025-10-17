// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetTPSCharacter.h"
#include "NetPlayer.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API ANetPlayer : public ANetTPSCharacter
{
	GENERATED_BODY()

public:
	ANetPlayer();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	
public:
	// 총 집기, 놓기 InputAction
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* takeGunAction;
	// 총을 붙일 Component
	UPROPERTY(EditAnywhere)
	class USceneComponent* compGun;
	// Level 에 있는 모든 총을 담을 변수
	UPROPERTY(EditAnywhere)
	TArray<class AActor*> allGun;
	// 총을 잡을 수 있는 범위
	UPROPERTY(EditAnywhere)
	float canTakeGunDist = 200;

	// 총을 집고 있는지 여부
	bool hasGun;
	// 집은 총 담을 변수
	UPROPERTY(ReplicatedUsing=AttachGun)
	class AGun* ownedGun;

	// 총 집기 / 놓기 함수
	void TakeGun();
	// [서버] 에게 총 집기 / 놓기 해달라고 요청
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeGun();
	// 총을 붙이는 함수
	UFUNCTION()
	void AttachGun();
	// 모든 [클라] 에게 총 탈착 요청 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DetachGun(class AGun* gun);
	// 총을 버리는 함수
	void DetachGun(class AGun* gun);


	// 총 집었을 때 CameraBoom 위치
	FVector cameraBoomLocationWithGun = FVector(0, 70, 110);
	// 총 집었을 때 Target Arm Length
	float targetArmLengthWithGun = 160;
	// 총 놓았을 때 CameraBoom 위치
	FVector cameraBoomLocationWithoutGun = FVector(0, 0, 110);
	// 총 놓았을 때 Target Arm Length
	float targetArmLengthWithoutGun = 400;
	// 총 집었는지 여부에 따라 CameraBoom 설정 변경 함수
	void ChangeCameraBoomSetting();


	// Player Montage
	UPROPERTY(EditAnywhere)
	class UAnimMontage* playerMontage;
	// 총 쏘기 InputAction
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* fireAction;
	// 재장전 InputAction
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* reloadAction;
	// 총 쏘기 함수
	void Fire();
	// [서버] 에게 총 쏘기 요청 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire();
	// 모든 [클라] 에게 총 쏴라
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_FireAction(bool bHit, FHitResult hitInfo, int32 combo);
	
	// 재장전 함수
	void Reload();
	// [서버] 에게 재장전 요청 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_Reload();
	// 모든 [클라] 에게 재장전 해라 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Reload();
	// 재장전 중 인지
	bool isReload;
	// 재장전 완료 함수
	void OnReloadComplete();

	// WBP_Main 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget> mainWidget;
	// 생성 된 WBP_Main 담을 변수
	UPROPERTY()
	class UMainWidget* mainUI;

	// 총 맞은 곳 Effect
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* hitEffect;

	// HPBar Widget 설정 할 Component
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* compHP;
	// 데미지 받았을 때 호출되는 함수
	void DamageProcess(float damage);
	// 머리 위 HPBar 가 항상 카메라를 바라보게 하는 함수
	void BillboardHPBar();

	// 죽었는지 여부
	bool isDie;
	// 죽었을 때 호출되는 함수
	void DieProcess();

	// 공격 시작 여부
	bool isFire;
	// 콤보 동작 여부
	bool isCombo;
	// 콤보 카운트
	int32 comboCnt;
	// 콤보가 진행될지 판단하는 함수
	void OnCombo();
	// 공격 마무리 (콤보 끊김 or 콤보 마지막 공격)
	void OnFireComplete();
	// 공격 액션
	void FireAction();

	// 서버 연결 상태 및 Onwer 설정 출력
	void PrintNetLog();

	// Possess 되었을 때 [클라] 에게 알려주는 함수
	UFUNCTION(Client, Reliable)
	void ClinetRPC_OnPossess();

	// BP_Cube 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> cubeFactory;
	// 큐브를 만들 수 있는지 여부
	UPROPERTY(Replicated)
	bool canMakeCube;
	// 큐브 만드는 함수
	void MakeCube();
	// [서버] 에게 큐브 만들어 달라는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_MakeCube();
};



















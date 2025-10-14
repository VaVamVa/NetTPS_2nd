// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/ColorConstants.h"
#include "NetActor.generated.h"

UCLASS()
class NETTPS_API ANetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Static Mesh Component
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	void PrintNetLog();

	// Owner 검색 범위
	float findDistance = 300;
	// Owner 설정 함수
	void FindOwner();

	// 회전 값 동기화 변수
	//UPROPERTY(Replicated)
	UPROPERTY(ReplicatedUsing=OnRep_Rotate)
	float rotYaw;
	// 회전 함수
	void Rotate();
	// rotYaw 값이 변경될 때 호출되는 함수
	UFUNCTION()
	void OnRep_Rotate();
	
	// Material 변수
	UPROPERTY()
	class UMaterialInstanceDynamic* mat;
	// Material 색 동기화 변수
	UPROPERTY(ReplicatedUsing=OnRep_ChangeColor)
	FLinearColor matColor;
	// 색 변경 함수
	void ChangeColor();
	// matColor 가 변경될 때 호출되는 함수
	UFUNCTION()
	void OnRep_ChangeColor();

	// 크기 변경 함수
	void ChangeScale();
	// 서버에게 크기 변경 요청
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeScale();
	// 클라에게 크기 변경 요청
	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeScale(float scale);
	// 모든 클라에게 크기 변경 요청
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ChangeScale(float scale);
};





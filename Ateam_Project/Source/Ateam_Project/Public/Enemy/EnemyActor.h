// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyData.h"
#include "Enemy/FallActor.h"
#include "Enemy/TurnMoveActor.h"
#include "Enemy/AttackZone.h"
#include "EnemyActor.generated.h"

class UBoxComponent;

UCLASS()
class ATEAM_PROJECT_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyActor();

	TObjectPtr<AAttackZone> GetZoneActor();
	FRotator GetActorRotator();
	float GetAttackActorSize();
	float GetAttackZoneSize();
	float GetAttackMoveSpeed();
	float GetAttackRotationSpeed();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetObjects();
	void SetBoxCollision();
	void SetAttackActor();
	void ResetAttack();
	void SpawnAttackActor();

	// 敵のアセットデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;

	// プレイヤーのアクター
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|PlayerPawn")
	TObjectPtr<APawn> PlayerPawn = nullptr;

	// 攻撃のオブジェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|TurnMoveActor")
	TSubclassOf<ATurnMoveActor> TurnMoveActor = nullptr;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|AttackZone")
	TSubclassOf<AAttackZone> AttackZone = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|GroundHeight")
	float GroundHeight = 0.0f;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	TObjectPtr<UBoxComponent> HeadCollision = nullptr;
	TObjectPtr<UBoxComponent> BodyCollision = nullptr;
	TObjectPtr<UBoxComponent> LegCollision = nullptr;

	TArray<FAttackState> AttackArray = {};
	TArray<ATurnMoveActor*> TurnMoveArray = {};

	FRotator ActorRotator = FRotator(0.f, 0.f, 0.f);
	float AttackActorSize = 0.0f;
	float AttackMoveSpeed = 0.0f;
	float AttackRotationSpeed = 0.0f;

	TObjectPtr<AAttackZone> ZoneActor = nullptr;
	float AttackZoneSize = 0.0f;
	
	float ReadyTime = 0.0f;
	float TotalTime = 0.0f;
	bool bIsValid = false;
	bool bIsReady = false;
};

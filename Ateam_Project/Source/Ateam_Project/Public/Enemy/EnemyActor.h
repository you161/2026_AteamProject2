// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyData.h"
#include "Enemy/FallActor.h"
#include "Enemy/TurnMoveActor.h"
#include "Enemy/AttackZone.h"
#include "Health/HealthManager.h"
#include "EnemyActor.generated.h"

class UBoxComponent;
class UDamageFlashComponent;
class ASEManager;

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

	void SetMoveActive();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	void SetObjects();
	void SetBoxCollision();
	void SetAttackActor();
	void AdvanceTurnMove(TArray<FVector> TLocation);
	void ResetAttack();
	void SpawnAttackActor();
	void Move(float DeltaTime);

	// 敵のアセットデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;

	// プレイヤーのアクター
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|PlayerPawn")
	TObjectPtr<APawn> PlayerPawn = nullptr;

	// 攻撃のオブジェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Attack|TurnMoveActor")
	TSubclassOf<ATurnMoveActor> TurnMoveActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Attack|FallActor")
	TSubclassOf<AFallActor> FallActor = nullptr;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Attack|AttackZone")
	TSubclassOf<AAttackZone> AttackZone = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|LevelGround")
	TObjectPtr<AActor> GroundActor = nullptr;

	//HPを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings|Health")
	TObjectPtr<AHealthManager> HealthManager = nullptr;

	//SEを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings|SE")
	TObjectPtr<ASEManager> SEManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|GroundHeight")
	float GroundHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Speed")
	float AngleSpeed = 0.0f;

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
	int32 TurnMoveCount = 0;

	FRotator ActorRotator = FRotator(0.f, 0.f, 0.f);
	float AttackActorSize = 0.0f;
	float AttackMoveSpeed = 0.0f;
	float AttackRotationSpeed = 0.0f;

	TObjectPtr<AAttackZone> ZoneActor = nullptr;
	float AttackZoneSize = 0.0f;

	FVector CenterLocation = FVector::Zero();
	FVector TargetLocation = FVector::Zero();
	TArray<FVector> PrevLocation = {};
	float Radius = 0.0f;
	float CurrentAngle = 0.0f;
	float MaxRarotation = 90.0f;
	
	float ReadyTime = 0.0f;
	float TotalTime = 0.0f;
	float RespawnTime = 0.0f;
	bool bIsValid = false;
	bool bIsReady = false;
	bool bIsMove = false;
	bool bIsReset = false;
	bool bIsRespawn = false;

	//ダメージを受けたときに呼ばれる
	void OnDamaged(ECharacterType DamagedType);
	//被ダメージ時の点滅コンポーネント
	UPROPERTY()
	TObjectPtr<UDamageFlashComponent> DamageFlashComponent = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnMoveActor.generated.h"

class UBoxComponent;
class AEnemyActor;
class AAttackZone;
class AEnemyManager;

UENUM(BlueprintType)
enum class ETurnMoveState : uint8
{
	Spawn = 0,
	Ready,
	Active
};

UCLASS()
class ATEAM_PROJECT_API ATurnMoveActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATurnMoveActor();

	FVector GetSpawnLocation();
	void MoveActive();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void MoveRoll();
	void MoveDrift();
	void MoveTurn(float DeltaTime);

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|回転しなくなる角度")
	float MaxRotation = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|生成時の演出時間")
	float SpawnTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|生成時の回転速度")
	float RollSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|生成後の漂う速度")
	float DriftTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|生成後の漂う範囲")
	float DriftRange = 0.0f;

private:
	ETurnMoveState TurnMoveState = ETurnMoveState::Spawn;
	TObjectPtr<AEnemyActor> EnemyActor = nullptr;
	TObjectPtr<AAttackZone> ZoneActor = nullptr;
	TObjectPtr<AEnemyManager> EnemyManager = nullptr;

	FVector BaseLocation = FVector::Zero();
	FVector StartLocation = FVector::Zero();
	FVector StartSize = FVector::Zero();
	FVector DriftLocation = FVector::Zero();

	float MaxActorSize = 0.0f;
	float MoveSpeed = 0.0f;
	float RotationSpeed = 0.0f;
	float countTime = 0.0f;

	bool bIsValid = false;
	bool bIsMove = false;
};

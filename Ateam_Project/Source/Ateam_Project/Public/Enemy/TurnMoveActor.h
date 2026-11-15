// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnMoveActor.generated.h"

class UBoxComponent;
class AEnemyActor;
class AAttackZone;
class AHealthManager;

UCLASS()
class ATEAM_PROJECT_API ATurnMoveActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATurnMoveActor();

	void MoveActive();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void TurnMove(float DeltaTime);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float MaxRotation = 10.0f;

private:
	TObjectPtr<AEnemyActor> EnemyActor = nullptr;
	TObjectPtr<AAttackZone> ZoneActor = nullptr;
	TObjectPtr<AHealthManager> HealthManager = nullptr;
	float MaxActorSize = 0.0f;
	float MoveSpeed = 0.0f;
	float RotationSpeed = 0.0f;
	float countTime = 0.0f;
	bool bIsValid = false;
	bool bIsMoveActive = false;
};

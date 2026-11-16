// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallActor.generated.h"

class UBoxComponent;
class AEnemyActor;
class AAttackZone;
class AHealthManager;

UCLASS()
class ATEAM_PROJECT_API AFallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFallActor();

	void MoveActive();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetBoxCollision();
	void Falling(float DeltaTime);
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|WiteTime")
	float WiteTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|FallHeight")
	float FallHeight = 1000.0f;

private:
	TObjectPtr<AEnemyActor> EnemyActor = nullptr;
	TObjectPtr<AAttackZone> ZoneActor = nullptr;
	TObjectPtr<AHealthManager> HealthManager = nullptr;
	TObjectPtr<UBoxComponent> BoxCollision = nullptr;

	float MaxActorSize = 0.0f;
	float FallSpeed = 0.0f;

	float countTime = 0.0f;
	float FallTime = 0.0f;

};

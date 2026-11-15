// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackZone.generated.h"

class AEnemyActor;
class USphereComponent;

UCLASS()
class ATEAM_PROJECT_API AAttackZone : public AActor
{
	GENERATED_BODY()
	
public:
	AAttackZone();

	bool GetIsHitPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void TickZone();

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SetTime = 0.05f;

private:
	FTimerHandle TimerHandle;
	FVector MaxActorSize = FVector::Zero();
	float MaxSphereRadius = 0.0f;
	float ZoneSize = 0.0f;
	float countTime = 0.0f;
	bool bIsValid = false;
	bool bIsHitPlayer = false;
};

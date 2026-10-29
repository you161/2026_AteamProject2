// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallActor.generated.h"

class UEnemyData;
class UStaticMeshComponent;
class UBoxComponent;
class UCapsuleComponent;

UCLASS()
class ATEAM_PROJECT_API AFallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFallActor();

	void SetHoming(bool flag);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetEnemyData();
	void SetBoxCollision();
	void SetPlayerPawn();
	void SetZoneMesh();
	void SetZoneCollision();
	void SetBaseSize();
	void SpawnZoneActor();
	void UpdateSize(float WaringTime);
	void UpdateLocation(float DeltaTime, float WarningTime, float FallSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;
	
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

private:
	TObjectPtr<AActor> PlayerPawn = nullptr;
	TObjectPtr<AActor> ZoneActor = nullptr;
	TObjectPtr<UStaticMeshComponent> ZoneMesh = nullptr;
	TObjectPtr<UCapsuleComponent> ZoneCollision = nullptr;
	TObjectPtr<UBoxComponent> BoxCollision = nullptr;
	FVector BaseSizeActor = FVector(0.0f, 0.0f, 0.0f);
	FVector BaseSizeMesh = FVector(0.0f, 0.0f, 0.0f);
	FVector BaseSizeColl = FVector(0.0f, 0.0f, 0.0f);
	float countTime = 0.0f;
	float FallTime = 0.0f;
	bool bIsConfigured = false;
	bool bIsHoming = false;
	bool bIsHitPlayer = false;
};

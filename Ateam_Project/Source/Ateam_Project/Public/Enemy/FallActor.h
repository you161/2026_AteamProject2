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

	void SetHoming();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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
	FVector BaseMesh = FVector(0.0f, 0.0f, 0.0f);
	FVector BaseCollision = FVector(0.0f, 0.0f, 0.0f);
	float countTime = 0.0f;
	float FallTime = 0.0f;
	bool bIsReady = false;
	bool bIsFalling = false;
	bool bIsHoming = false;
	bool bIsHitPlayer = false;
};

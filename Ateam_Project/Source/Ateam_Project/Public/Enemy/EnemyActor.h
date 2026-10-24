// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

class UEnemyData;
class AFallActor;
class UBoxComponent;

UCLASS()
class ATEAM_PROJECT_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetEnemyData();
	void SetBoxCollision();
	void SpawnRandom();
	void SpawnHoming();

	// 敵のアセットデータ
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

private:
	TObjectPtr<UBoxComponent> BoxCollision = nullptr;
	float RcountTime = 0.0f;
	float HcountTime = 0.0f;
	bool bIsConfigured = false;

};

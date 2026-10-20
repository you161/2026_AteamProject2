// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

class UEnemyData;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPawn")
	TObjectPtr<AActor> PlayerPawn = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackActor")
	TSubclassOf<AActor> BP_AttackActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackActor")
	FVector AttackActorLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* AttackCollsition;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	TObjectPtr<AActor> AttackActor = nullptr;
	FVector TargetDirection = FVector(0.0f, 0.0f, 0.0f);

	bool bIsAttack = false;
};

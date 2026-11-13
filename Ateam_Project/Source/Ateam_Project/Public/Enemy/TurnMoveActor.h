// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyData.h"	
#include "TurnMoveActor.generated.h"

class UBoxComponent;

UCLASS()
class ATEAM_PROJECT_API ATurnMoveActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATurnMoveActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetPlayerPawn();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float RotationSpeed = 1.0f;

private:
	TObjectPtr<AActor> PlayerPawn = nullptr;
	bool bIsConfigured = false;

	void Move(float DeltaTime);
};

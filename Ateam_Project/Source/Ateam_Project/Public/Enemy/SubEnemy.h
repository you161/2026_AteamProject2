// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SubEnemy.generated.h"

class UEnemyData;

UCLASS()
class ATEAM_PROJECT_API ASubEnemy : public APawn
{
	GENERATED_BODY()

public:
	ASubEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPlayerPawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyData")
	float MoveSpeed = 100.0f;

private:
	TObjectPtr<AActor> PlayerPawn = nullptr;
	bool bIsConfigured = false;
};

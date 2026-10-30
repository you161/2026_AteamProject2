// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

class UHitPointBar;

UCLASS()
class ATEAM_PROJECT_API AUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AUIManager();

	TObjectPtr<UHitPointBar> GetPlayerHPBar();
	TObjectPtr<UHitPointBar> GetEnemyHPBar();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHitPointBar> PlayerHPBarClass;

	UPROPERTY()
	TObjectPtr<UHitPointBar> PlayerHPBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHitPointBar> EnemyHPBarClass;

	UPROPERTY()
	TObjectPtr<UHitPointBar> EnemyHPBar;

public:

};

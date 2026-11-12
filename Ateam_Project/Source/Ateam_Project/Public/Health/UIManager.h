// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

class UHitPointBar;
class AHealthManager;
class UStaminaWidget;
class AStaminaManager;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager")
	TObjectPtr<AHealthManager> HealthManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UStaminaWidget> StaminaWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UStaminaWidget> StaminaWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "StaminaManager")
	TObjectPtr<AStaminaManager> StaminaManager = nullptr;

public:

};

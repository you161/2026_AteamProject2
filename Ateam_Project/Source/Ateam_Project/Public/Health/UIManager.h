// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

class UHitPointBar;
class AHealthManager;
class UStaminaWidget;
class AStaminaManager;

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	UserWidget = 0,
	PlayerHBar,
	EnemyHPBar,
	Stamina
};

USTRUCT(BlueprintType)
struct FWidgetEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWidgetType WidgetType = EWidgetType::UserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass = nullptr;
};

UCLASS()
class ATEAM_PROJECT_API AUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AUIManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager")
	TObjectPtr<AHealthManager> HealthManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StaminaManager")
	TObjectPtr<AStaminaManager> StaminaManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<FWidgetEntry> WidgetEntry = {};

private:
	TArray<TObjectPtr<UUserWidget>> SpawnedWidgets = {};

};

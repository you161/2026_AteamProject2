// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/UIManager.h"
#include "Health/HitPointBar.h"
#include "Health/HealthManager.h"
#include "UI/StaminaWidget.h"
#include "Player/StaminaManager.h"

AUIManager::AUIManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUIManager::BeginPlay()
{
	Super::BeginPlay();	

	if (PlayerHPBarClass)
	{
		PlayerHPBar = CreateWidget<UHitPointBar>(GetWorld(), PlayerHPBarClass);
		if (PlayerHPBar)
		{
			PlayerHPBar->AddToViewport();
		}
	}
	if (EnemyHPBarClass)
	{
		EnemyHPBar = CreateWidget<UHitPointBar>(GetWorld(), EnemyHPBarClass);
		if (EnemyHPBar)
		{
			EnemyHPBar->AddToViewport();
		}
	}

	if(IsValid(HealthManager))
	{
		HealthManager->InitializeHealth(PlayerHPBar, EnemyHPBar);
	}

	if (StaminaWidgetClass)
	{
		StaminaWidget = CreateWidget<UStaminaWidget>(GetWorld(), StaminaWidgetClass);
		if (IsValid(StaminaWidget) && IsValid(StaminaManager))
		{
			StaminaWidget->AddToViewport();
			StaminaWidget->SetStaminaManager(StaminaManager);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StaminaWidget or StaminaManager is not valid in UIManager."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StaminaWidgetClass is not set in UIManager."));
	}
}

void AUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TObjectPtr<UHitPointBar> AUIManager::GetPlayerHPBar()
{
	return PlayerHPBar;
}

TObjectPtr<UHitPointBar> AUIManager::GetEnemyHPBar()
{
	return EnemyHPBar;
}
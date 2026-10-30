// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "HitPointBar.h"

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
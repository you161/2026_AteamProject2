// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyData.h"

UEnemyData::UEnemyData() {}

bool UEnemyData::IsConfigured() const
{
	if (bHasHoming && PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in EnemyData."));
		return false;
	}
	if (bHasRandom && RFallActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomActor is not set in EnemyData."));
		return false;
	}
	if (bHasHoming && HFallActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HomingActor is not set in EnemyData."));
		return false;
	}
	if (DamageZone == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageZone is not set in EnemyData."));
		return false;
	}
	if (!bHasRandom && !bHasHoming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack is not enable in EnemyData"));
		return false;
	}

	return true;
}
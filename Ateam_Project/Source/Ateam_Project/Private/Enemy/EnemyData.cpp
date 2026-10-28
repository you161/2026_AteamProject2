// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyData.h"

UEnemyData::UEnemyData() {}

bool UEnemyData::IsConfigured() const
{
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in EnemyData."));
		return false;
	}
	if (bHasRandom && RFallActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomActor is not set in EnemyData."));
		return false;
	}
	if (bHasRandom && RandomZone == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomZone is not set in EnemyData."));
		return false;
	}
	if (bHasHoming && HFallActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HomingActor is not set in EnemyData."));
		return false;
	}
	if (bHasHoming && HomingZone == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HomingZone is not set in EnemyData."));
		return false;
	}
	if (!bHasRandom && !bHasHoming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack is not enable in EnemyData"));
		return false;
	}

	return true;
}
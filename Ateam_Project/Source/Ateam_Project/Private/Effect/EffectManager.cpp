// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EffectManager.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AEffectManager::AEffectManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEffectManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(EffectData))
	{
		UE_LOG(LogTemp, Error, TEXT("EffectData is not set in EffectManager."));
	}
}

void AEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffectManager::PlayEffect(EEffectType Type, FVector Location)
{
	if (!IsValid(EffectData))
	{
		return;
	}

	FEffectEntry* Entry = EffectData->EffectMap.Find(Type);

	if (!Entry || !Entry->Effect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect is not set : %d"), static_cast<int32>(Type));
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), Entry->Effect, Location);
}
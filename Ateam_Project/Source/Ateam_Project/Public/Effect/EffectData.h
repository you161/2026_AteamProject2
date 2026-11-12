// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EffectData.generated.h"

/**
 *  エフェクトのデータアセット
 */

class UNiagaraSystem;

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	ObjectCollect = 0,
	PlayerDamage,
	EnemyDamage
};

USTRUCT(BlueprintType)
struct FEffectEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> Effect = nullptr;
};

UCLASS()
class ATEAM_PROJECT_API UEffectData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UEffectData();

	// エフェクト管理配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TMap<EEffectType, FEffectEntry> EffectMap = {};
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

/**
 *   敵のデータアセットクラス
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 攻撃の速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackActor")
	float AttackSpeed = 200.0f;
};

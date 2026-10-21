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
	// 攻撃の数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quantity")
	int32 ActorCount = 1;

	// 生成初期位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Location")
	FVector AttackActorLocation = FVector(0.0f, 0.0f, 0.0f);

	// 攻撃の時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	float CooldownTime = 0.0f;

	// 攻撃の速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float MoveSpeed = 200.0f;
};

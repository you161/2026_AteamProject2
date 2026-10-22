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
	// 攻撃のオブジェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	TSubclassOf<AActor> BP_FallActor = nullptr;

	// 攻撃の数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	int32 ActorCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float SetHeight = 1000.0f;

	// 生成エリア
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	TArray<FVector4> SetArea{ FVector4(-1000.0f, -1000.0f, 1000.0f, 1000.0f) };

	// 攻撃の時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float CooldownTime = 0.0f;

	// 攻撃の速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float MoveSpeed = 200.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

/**
 *   敵のデータアセットクラス
 */

UENUM(BlueprintType)
enum class EAttackSeed : uint8
{
	RandomAttack = 0,
	HomingAttack
};

USTRUCT(BlueprintType)
struct FAttackState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "時間"))
	float AttackTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃の種類"))
	EAttackSeed AttackSeed = EAttackSeed::RandomAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃アクタのサイズ"))
	float ActorSize = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃範囲のサイズ"))
	float ZoneSize = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "移動速度"))
	float MoveSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "回転速度"))
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "初めに回転させるランダム範囲  X = Min Y = Max"))
	FVector2D PitchYawRange = FVector2D::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadOnly,
		meta = (DisplayName = "生成する範囲  X = MinX Y = MinY Z = MaxX W = MaxY"),
		meta = (EditCondition = "AttackSeed == EAttackSeed::RandomAttack"))
	FVector4 SpawnArea = FVector4::Zero();
};

UCLASS(BlueprintType)
class ATEAM_PROJECT_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UEnemyData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "攻撃の設定")
	TArray<FAttackState> AttackState = {};
};

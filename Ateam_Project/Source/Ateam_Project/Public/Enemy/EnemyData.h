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
	HomingAttack,
	Reset,
};

USTRUCT(BlueprintType)
struct FAttackState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "時間"))
	float AttackTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃の種類"))
	EAttackSeed AttackSeed = EAttackSeed::RandomAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃を上から落とすか"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset"))
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃アクタのサイズ"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset"))
	float ActorSize = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "攻撃範囲のサイズ"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset"))
	float ZoneSize = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "移動速度"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset"))
	float MoveSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "回転速度"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset && !bIsFalling"))
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "移動開始時の向き(上下)  X = Min Y = Max"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset && !bIsFalling"))
	FVector2D PitchRange = FVector2D::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "移動開始時の向き(左右)  X = Min Y = Max"),
		meta = (EditCondition = "AttackSeed != EAttackSeed::Reset && !bIsFalling"))
	FVector2D YawRange = FVector2D::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "生成する範囲  X = MinX Y = MinY Z = MaxX W = MaxY"),
		meta = (EditCondition = "AttackSeed == EAttackSeed::RandomAttack"))
	FVector4 SpawnArea = FVector4::Zero();
};

UCLASS(BlueprintType)
class ATEAM_PROJECT_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UEnemyData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "待機時の設定|X = 本体との距離 Y = 攻撃との距離 Z = 高さの範囲")
	FVector AttackDistance = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "待機時の設定|個数")
	int32 Count = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "待機時の設定|初めの生成までの時間")
	float SpawnTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "待機時の設定|次の生成までの時間")
	float WiteTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "攻撃の設定")
	TArray<FAttackState> AttackState = {};
};

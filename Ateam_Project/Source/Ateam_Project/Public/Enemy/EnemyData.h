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
	UEnemyData();
	bool IsConfigured() const;

	//=======================================================================

	// プレイヤーのアクター
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPawn")
	TSubclassOf<AActor> PlayerPawn = nullptr;

	//=======================================================================

	// ランダム生成を有効にするか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	bool bHasRandom = true;

	// 攻撃のオブジェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	TSubclassOf<AActor> RFallActor = nullptr;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	TSubclassOf<AActor> RandomZone = nullptr;

	// ランダム攻撃の予告時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float RWarningTime = 1.0f;

	// 攻撃の数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	int32 ActorCount = 1;

	// 生成エリア
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	TArray<FVector4> RSetArea{ FVector4(-1000.0f, -1000.0f, 1000.0f, 1000.0f) };

	// 生成高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float RFallHeight = 1000.0f;

	// 攻撃の速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float RFallSpeed = 200.0f;

	// 攻撃の時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomAttack")
	float RCooldownTime = 0.0f;

	//=======================================================================

	// 追従攻撃の生成を有効にするか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	bool bHasHoming = true;

	// 攻撃のオブジェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	TSubclassOf<AActor> HFallActor = nullptr;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	TSubclassOf<AActor> HomingZone = nullptr;

	// 追従速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	float HomingSpeed = 5.0f;

	// 追従攻撃の予告時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	float HWarningTime = 1.0f;

	// 生成高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	float HFallHeight = 1000.0f;

	// 攻撃の速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	float HFallSpeed = 200.0f;

	// 攻撃の時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingAttack")
	float HCooldownTime = 0.0f;
};

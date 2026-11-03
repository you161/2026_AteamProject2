
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DamageAmountData.generated.h"

/**
 * ダメージ量データを保持するデータアセットクラス
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UDamageAmountData : public UDataAsset
{
	GENERATED_BODY()

public:
	//プレイヤーが与えるダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TArray<float> PlayerAttackDamage = {};

	//敵が与えるダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float EnemyAttackDamage = 0.0f;

};
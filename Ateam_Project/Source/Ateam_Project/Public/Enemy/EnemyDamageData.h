#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDamageData.generated.h"

/**
 * 敵のダメージ量データを保持するデータアセットクラス
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UEnemyDamageData : public UDataAsset
{
	GENERATED_BODY()

public:
	//敵が与えるダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float EnemyAttackDamage = 0.0f;
};
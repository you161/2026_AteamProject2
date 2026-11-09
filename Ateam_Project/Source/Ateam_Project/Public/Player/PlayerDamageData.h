
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerDamageData.generated.h"

/**
 * ダメージ量データを保持するデータアセットクラス
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UPlayerDamageData : public UDataAsset
{
	GENERATED_BODY()

public:
	//プレイヤーが与えるダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TArray<float> PlayerAttackDamage = {};
};
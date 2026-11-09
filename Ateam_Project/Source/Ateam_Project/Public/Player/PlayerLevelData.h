
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerLevelData.generated.h"

/**
 * プレイヤーのレベルデータを管理するデータアセット
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UPlayerLevelData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float>MaxGaugeTable = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> GaugeGainTable = {};
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HealthData.generated.h"

/**
 * HPを管理するデータアセット
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UHealthData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//プレイヤーの最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP|Player")
	float PlayerMaxHealth = 0.0f;
	//敵の最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP|Enemy")
	float EnemyMaxHealth = 0.0f;
};
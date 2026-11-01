
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ThrowableObjectData.generated.h"

/**
 * 投げれるオブジェクトのデータを格納するためのデータアセットクラス
 */
UCLASS(BlueprintType)
class UThrowableObjectData : public UDataAsset
{
	GENERATED_BODY()
public:
	//投げられるオブジェクトの移動速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	float MoveSpeed = 500.0f;

	//投げてから消えるまでの飛距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	float MaxThrowDistance = 3000.0f;

	//持たれているときのオフセット
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	FVector Offset = FVector(100.0f, 0.0f, 50.0f);

	//1個回収するごとに増えるスケール倍率
	UPROPERTY(EditAnywhere, Category = "Throwable")
	float ScalePerItem = 0.2f;

	//最大スケール
	UPROPERTY(EditAnywhere, Category = "Throwable")
	float MaxScale = 5.0f;

	//移動中の回転速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	FRotator RotateSpeed = FRotator(0.0f, 180.0f, 0.0f);
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerData.generated.h"

/**
 * プレイヤーのデータを格納するためのデータアセットクラス
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UPlayerData : public UDataAsset
{
	GENERATED_BODY()
public:
	//プレイヤーの移動速度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float MoveSpeed = 500.0f;

	//プレイヤーの回転速度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate")
    float RotateSpeed = 500.0f;

	//プレイヤーのジャンプ速度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float JumpSpeed = 600.0f;

	//プレイヤーの重力
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float Gravity = 980.0f;

	//カメラの感度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    float CameraSensitivity = 1.0f;

	//カメラの距離
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    float CameraDistance = 400.0f;

	//カメラの高さオフセット
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    float CameraHeightOffset = 50.0f;

	//カメラの最小ピッチ角度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    float MinPitch = -60.0f;

	//カメラの最大ピッチ角度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    float MaxPitch = 20.0f;
};


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerData.generated.h"

/*
* プレイヤーのデータを格納するための構造体
*/
USTRUCT(BlueprintType)
struct FMoveSpeed
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<float> Values = {};
};

/**
 * プレイヤーのデータを格納するためのデータアセットクラス
 */
UCLASS(BlueprintType)
class ATEAM_PROJECT_API UPlayerData : public UDataAsset
{
	GENERATED_BODY()
public:
	//プレイヤーの移動速度
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMoveSpeed> MoveSpeedTable = {};

	//ダッシュ時の速度倍率
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|ダッシュ")
    float DashSpeedRate = 1.5f;

	//プレイヤーの回転速度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate|回転速度")
    float RotateSpeed = 500.0f;

	//カメラの感度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|感度")
    float CameraSensitivity = 1.0f;

	//カメラの距離
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|距離")
    float CameraDistance = 400.0f;

	//カメラの高さオフセット
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|高さ")
    float CameraHeightOffset = 50.0f;

	//カメラの最小ピッチ角度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|最低ピッチ")
    float MinPitch = -60.0f;

	//カメラの最大ピッチ角度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|最大ピッチ")
    float MaxPitch = 20.0f;

    //カメラの最小ヨー角度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|最低ヨー")
    float MinYaw = -60.0f;

    //カメラの最大ヨー角度
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|最大ヨー")
    float MaxYaw = 60.0f;
};
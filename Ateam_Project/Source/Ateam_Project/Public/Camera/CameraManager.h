
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

class UPlayerData;
class UInputManager;

/**
 * カメラ管理クラス
 */
UCLASS()
class ATEAM_PROJECT_API ACameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraManager();

protected:
	virtual void BeginPlay() override;

	//InputManagerを初期化する関数
	void InitializeInputManager();

public:	
	virtual void Tick(float DeltaTime) override;

	//カメラの回転を処理する関数
	void RotateCamera();

protected:
	//プレイヤーのデータを格納するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UPlayerData> PlayerData = nullptr;

	//カメラを制御するアクター
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<AActor> CameraActor = nullptr;
	
	//カメラが注視するターゲットのアクター
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<AActor> TargetActor = nullptr;

private:
	//入力管理を行うオブジェクト
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;
	//カメラの現在の方向を保持する変数
	FVector CurrentDirection = FVector::ZeroVector;

	//カメラの回転角度を保持する変数
	float Yaw = 0.0f;
	//カメラの回転角度を保持する変数
	float Pitch = 0.0f;
};
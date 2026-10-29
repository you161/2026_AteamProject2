#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UInputManager;
class UPlayerData;
class UCapsuleComponent;

/**
 * 入力の値を受け取り、プレイヤーの移動やジャンプの状態を管理するクラス
 */
UCLASS()
class ATEAM_PROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

	virtual void Tick(float DeltaTime) override;

	void SetHasObject(bool CurrentFlag);

protected:
	virtual void BeginPlay() override;

	//コントローラーがPossessしたときに呼ばれる
	virtual void PossessedBy(AController* NewController) override;
	//コントローラーがUnPossessしたときに呼ばれる
	virtual void UnPossessed() override;

private:

	//InputManagerを初期化する関数
	void InitializeInputManager();
	//移動処理を行う関数
	void Move(float DeltaTime);
	//回転処理を行う関数
	void Rotate(float DeltaTime);

protected:

	//プレイヤーのデータを格納するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UPlayerData> PlayerData = nullptr;
	//カメラを制御するアクター
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<AActor> Camera = nullptr;
	//プレイヤーの当たり判定を行うコリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<UCapsuleComponent> CapsuleCollision;

private:

	//入力管理を行うオブジェクト
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;

	//カメラの現在の方向を保持する変数
	FVector CurrentDirection = FVector::ZeroVector;
	//カメラの現在の前方向を保持する変数
	FVector CurrentCameraForward = FVector::ZeroVector;
	//カメラの現在の右方向を保持する変数
	FVector CurrentCameraRight = FVector::ZeroVector;
	//プレイヤーが移動中かどうかを保持する変数
	bool isMoving = false;
	//減速中
	bool hasObject = false;
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Health/CharacterType.h"
#include "Player/PlayerMoveState.h"
#include "PlayerPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerPawn, Log, All);
DECLARE_MULTICAST_DELEGATE(FOnPlayerThrow);

class UInputManager;
class UPlayerData;
class UCapsuleComponent;
class AThrowableManager;
class APlayerLevelManager;
class AStaminaManager;
class AHealthManager;
class UDamageFlashComponent;

/**
 * 入力の値を受け取り、プレイヤーの移動や回転を管理するクラス
 */
UCLASS()
class ATEAM_PROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

	virtual void Tick(float DeltaTime) override;
	//移動中かどうかを取得する関数
	bool GetIsMoving()const;
	//プレイヤーの移動状態を取得する関数
	EPlayerMoveState GetMoveState() const { return MoveState; }
	//プレイヤーがオブジェクトを所持しているかを取得する関数
	bool GetHasThrowable() const;
	//プレイヤーが投げたときに通知するデリゲート
	FOnPlayerThrow OnThrow;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//コントローラーがPossessしたときに呼ばれる
	virtual void PossessedBy(AController* NewController) override;
	//コントローラーがUnPossessしたときに呼ばれる
	virtual void UnPossessed() override;

private:
	//InputManagerを初期化する関数
	void InitializeInputManager();
	//必要な参照が設定されているか確認してログに出す関数
	void ValidateReferences() const;
	//Tickで必要な参照がそろっているかを返す関数
	bool HasRequiredReferences() const;
	//移動処理を行う関数
	void Move(float DeltaTime);
	//回転処理を行う関数
	void Rotate(float DeltaTime);
	//入力とカメラの向きから移動方向を更新する関数
	void UpdateMoveDirection(const FVector2D& MoveInput);
	//現在のレベルと所持数に応じた基本移動速度を返す関数
	float GetMoveSpeed() const;
	//衝突したら壁に沿って滑るように移動する関数
	void MoveWithSlide(const FVector& Delta);
	//ダメージを受けたときに呼ばれる
	void OnDamaged(ECharacterType DamagedType);
	//ThrowableManagerからの通知を受けて、自分の通知を飛ばす
	void HandleThrowStarted();

protected:
	//プレイヤーのデータを格納するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PlayerPawn|Data")
	TObjectPtr<UPlayerData> PlayerData = nullptr;
	//スタミナを制御するアクター
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PlayerPawn|Stamina")
	TObjectPtr<AStaminaManager> StaminaManager = nullptr;
	//カメラを制御するアクター
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PlayerPawn|Camera")
	TObjectPtr<AActor> Camera = nullptr;
	//拾ったオブジェクトを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PlayerPawn|Throwable")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;
	//プレイヤーのレベルを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PlayerPawn|Level")
	TObjectPtr<APlayerLevelManager> PlayerLevelManager = nullptr;
	//HPを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "PlayerPawn|Health")
	TObjectPtr<AHealthManager> HealthManager = nullptr;
	//プレイヤーの当たり判定を行うコリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<UCapsuleComponent> CapsuleCollision = nullptr;

private:
	//入力管理を行うオブジェクト
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;
	//被ダメージ時の点滅コンポーネント
	UPROPERTY(Transient)
	TObjectPtr<UDamageFlashComponent> DamageFlashComponent = nullptr;
	//現在の移動方向(水平・正規化済み)
	FVector CurrentDirection = FVector::ZeroVector;
	//現在の移動状態
	EPlayerMoveState MoveState = EPlayerMoveState::Idle;
	bool bIsMoving = false;
};
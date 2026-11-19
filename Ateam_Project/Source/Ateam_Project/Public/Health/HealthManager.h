#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Health/CharacterType.h"
#include "Enemy/EnemyManager.h"
#include "HealthManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHealth, Log, All);

class UHitPointBar;
class AGameJudge;
class UPlayerDamageData;
class UEnemyDamageData;
class UHealthData;
class AThrowableManager;

//ダメージを受けた時に通知するデリゲート
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDamaged, ECharacterType, int32);

/**
 * HP管理クラス。
 */
UCLASS()
class ATEAM_PROJECT_API AHealthManager : public AActor
{
	GENERATED_BODY()

public:
	AHealthManager();
	//指定したキャラクターに指定した量のダメージを与える
	void ApplyDamage(ECharacterType Target, float DamageAmount, int32 DamageLevel = 0);
	//HPバー設定
	void InitializeHealth(UHitPointBar* PlayerBar, UHitPointBar* EnemyBar);
	//現在のHPを取得する
	float GetHealth(ECharacterType Target) const;
	//ダメージを受けた時に呼ばれるデリゲート
	FOnDamaged OnDamaged;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	//HPの配列を初期化する
	void EnsureInitialized();
	//HPバーを関連付けて、空になった通知を受け取る
	void BindHitPointBar(ECharacterType Target, UHitPointBar* Bar);
	//プレイヤーに敵の攻撃が当たったときに呼び出される
	void HandleEnemyHitPlayer(EEnemyAttackType AttackType);
	//投擲オブジェクトが敵に当たった時に呼ばれる
	void HandleThrowableHitEnemy(int32 StackCount);
	//HPバーが空になった時に呼ばれる
	void OnHPBarEmptied(UHitPointBar* EmptiedBar);

protected:
	//プレイヤーの攻撃力を管理するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|PlayerDamage")
	TObjectPtr<UPlayerDamageData> PlayerDamageData = nullptr;
	//敵の攻撃力を管理するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|EnemyDamage")
	TObjectPtr<UEnemyDamageData> EnemyDamageData = nullptr;
	//HPを管理するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|Health")
	TObjectPtr<UHealthData> HealthData = nullptr;
	//ゲームの勝敗を判定するクラス
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|GameJudge")
	TObjectPtr<AGameJudge> GameJudge = nullptr;
	//敵に当たった通知を受け取るマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|ThrowableManager")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;
	//プレイヤーに当たった通知を受け取るマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|EnemeyManager")
	TObjectPtr<AEnemyManager> EnemyManager = nullptr;

private:
	//現在のHP
	TArray<float> CurrentHealth = {};
	//最大HP
	TArray<float> MaxHealth = {};
	//HPバー
	TArray<TWeakObjectPtr<UHitPointBar>> HitPointBars = {};
	//初期化済みかどうか
	bool bIsInitialized = false;
	//ゲーム終了の判定が済んだかどうか
	bool bIsGameFinished = false;
};
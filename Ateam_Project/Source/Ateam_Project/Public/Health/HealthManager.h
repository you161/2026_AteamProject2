
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthManager.generated.h"

class UHitPointBar;
class AGameJudge;
class UPlayerDamageData;
class UEnemyDamageData;
class UHealthData;
class AThrowableManager;

enum CharactorType
{
	Player = 0,
	Enemy = 1,
};

/*
* HP管理クラス
*/
UCLASS()
class ATEAM_PROJECT_API AHealthManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AHealthManager();

protected:

	virtual void BeginPlay() override;

public:	
	//ダメージ分HPを減らす関数
	void TakeDamage(CharactorType CurrentCharactorType);
	//初期化処理
	void InitializeHealth(UHitPointBar* PlayerBar, UHitPointBar* EnemyBar);
	//HP参照関数
	float GetHealth(CharactorType CurrentCharactorType)const;

private:

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager|GameJudge")
	TObjectPtr<AGameJudge> GameJudge = nullptr;

	//拾ったオブジェクトを管理するマネージャー
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager|Throwable")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;

private:
	//現在のHPを保存する変数
	TArray<float> CurrentHealth = {0,0};
	TArray<float> TotalDamage = { 0,0 };
	TArray<float> MaxHealth = { 0,0 };
	TArray<TObjectPtr<UHitPointBar>> HitPointBar = {nullptr,nullptr};

	bool bIsGameFinished = false;

	int32 CollectedCount = 0;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthManager.generated.h"

class UHitPointBar;
class AGameJudge;
class UDamageAmountData;
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
	//最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager|HP|0がPlayer,1がEnemy")
	TArray<float> MaxHealth = {0,0};

	//拾ったオブジェクトを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "HealthManager|Damage")
	TObjectPtr<UDamageAmountData> DamageAmountData = nullptr;

	//ゲームの勝敗を判定するクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager|GameJudge")
	TObjectPtr<AGameJudge> GameJudge = nullptr;

	//拾ったオブジェクトを管理するマネージャー
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager|Throwable")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugDamage = true;

private:
	//現在のHPを保存する変数
	TArray<float> CurrentHealth = {0,0};
	TArray<float> TotalDamage = { 0,0 };
	TArray<TObjectPtr<UHitPointBar>> HitPointBar = {nullptr,nullptr};

	bool bIsGameFinished = false;

	int32 CollectedCount = 0;
};
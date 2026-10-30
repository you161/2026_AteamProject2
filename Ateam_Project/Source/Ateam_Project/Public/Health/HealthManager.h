
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthManager.generated.h"

class UHitPointBar;
class AGameJudge;

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
	int32 GetHealth(CharactorType CurrentCharactorType)const;

private:

	void OnHPBarEmptied(UHitPointBar* EmptiedBar);

protected:
	//最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP|0がPlayer,1がEnemy")
	TArray<int32> MaxHealth = {0,0};
	//受けるダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP|0がPlayer,1がEnemy")
	TArray<int32> DamageAmount = {0,0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameJudge")
	TObjectPtr<AGameJudge> GameJudge = nullptr;

private:
	//現在のHPを保存する変数
	TArray<int32> CurrentHealth = {0,0};
	TArray<int32> TotalDamage = { 0,0 };
	TArray<TObjectPtr<UHitPointBar>> HitPointBar = {nullptr,nullptr};

	bool bIsGameFinished = false;
};
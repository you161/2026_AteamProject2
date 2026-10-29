
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthManager.generated.h"

class UHitPointBar;

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
	void TakeDamage();
	//HP参照関数
	int32 GetHealth()const;

protected:
	//最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	int32 MaxHealth = 0;
	//ダメージ量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	int32 DamageAmount = 0;

private:
	//現在のHPを保存する変数
	int32 CurrentHealth = 0;

};
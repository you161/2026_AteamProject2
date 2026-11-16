#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

//敵の攻撃の種類
UENUM(BlueprintType)
enum class EEnemyAttackType : uint8
{
	Normal,
	Count UMETA(Hidden)
};

//プレイヤーが被弾したことを通知するデリゲート
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitPlayer, EEnemyAttackType);

//敵からプレイヤーへの攻撃ヒット通知する
UCLASS()
class AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	AEnemyManager();

	//敵が攻撃を当てたときに呼ぶ
	void NotifyHitPlayer(EEnemyAttackType AttackType);
	//プレイヤーが被弾したときの通知
	FOnHitPlayer OnHitPlayer;
};
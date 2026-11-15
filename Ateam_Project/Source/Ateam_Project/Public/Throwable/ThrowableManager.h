#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThrowable, Log, All);

class AThrowableObject;
class UInputManager;
class UPlayerDamageData;

// 投げたオブジェクトが敵に当たった時に通知するデリゲート
DECLARE_MULTICAST_DELEGATE_OneParam(FOnThrowableHitEnemy, int32 StackCount);

/**
 * 拾ったオブジェクトを管理するマネージャークラス。
 */
UCLASS()
class ATEAM_PROJECT_API AThrowableManager : public AActor
{
	GENERATED_BODY()

public:
	AThrowableManager();

	//アイテムを拾えたらtrueを返す
	bool PickUp(AThrowableObject* Target);
	//投げたアイテムが敵に当たったことを通知する
	void NotifyHitEnemy(int32 StackCount);
	//現在持っている個数を取得する
	int32 GetCollectedCount() const { return CollectedCount; }
	//投げたアイテムが敵に当たった時に呼ばれるデリゲート
	FOnThrowableHitEnemy OnHitEnemy;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	//InputManagerを取得して投擲入力を購読する
	void InitializeInputManager();
	//投擲入力が来た時に呼ばれる
	void HandleInteract();
	//持っているアイテムを投げる
	void Throw();

protected:
	//プレイヤー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager|Player")
	TObjectPtr<AActor> Player = nullptr;
	//所持できる上限を決めるためのデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager|Damage")
	TObjectPtr<UPlayerDamageData> PlayerDamageData = nullptr;

private:
	//現在持っているアイテム
	UPROPERTY(Transient)
	TObjectPtr<AThrowableObject> HeldItem = nullptr;
	//入力管理を行うオブジェクト
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;
	//回収したアイテム数
	int32 CollectedCount = 0;
};
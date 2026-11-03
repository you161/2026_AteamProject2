
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableManager.generated.h"

class AThrowableObject;
class UInputManager;
class UDamageAmountData;

/**
 * 拾ったオブジェクトマネージャークラス
 */
UCLASS()
class ATEAM_PROJECT_API AThrowableManager : public AActor
{
	GENERATED_BODY()
	
public:

	AThrowableManager();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	//アイテムを拾った時の処理
	void PickUp(AThrowableObject* Target);

	//現在持っているアイテムを取得する関数
	int32 GetCollectedCount() const { return CollectedCount; }

	//投げた時の個数を取得する関数
	int32 GetThrownCount() const { return ThrownCount; }

private:

	//InputManagerを初期化する関数
	void InitializeInputManager();
	//アイテムを投げた時の処理
	void Throw();

protected:

	//プレイヤー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager|Player")
	TObjectPtr<AActor> Player = nullptr;
	//ダメージ量データを保持するデータアセット
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager|Damage")
	TObjectPtr<UDamageAmountData> DamageAmountData = nullptr;

private:
	//現在持っているアイテム
	UPROPERTY(Transient)
	TObjectPtr<AThrowableObject> HeldItem = nullptr;

	//入力管理を行うオブジェクト
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;

	//回収したアイテム数
	int32 CollectedCount = 0;

	//最後に投げた時の個数
	int32 ThrownCount = 0;
};
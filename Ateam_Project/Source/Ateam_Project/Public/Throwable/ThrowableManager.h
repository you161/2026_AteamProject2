
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableManager.generated.h"

class AThrowableObject;
class UInputManager;

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

	//アイテムを投げた時の処理
	int32 GetCollectedCount() const { return CollectedCount; }

private:

	//InputManagerを初期化する関数
	void InitializeInputManager();
	//アイテムを投げた時の処理
	void Throw();

protected:

	//プレイヤー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager|Player")
	TObjectPtr<AActor> Player = nullptr;
	//拾ったオブジェクトの最大数
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager|MaxObjectCount")
	int32 MaxObjectCount = 0;

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
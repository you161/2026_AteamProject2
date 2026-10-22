
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableManager.generated.h"

class AThrowableObject;
class UInputManager;

/**
 * 近くのアイテムを拾うためのマネージャークラス
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

	void AddCandidate(AThrowableObject* Item);
	void RemoveCandidate(AThrowableObject* Item);

private:

	//候補の中から一番近いアイテムを返す(なければnullptr)
	AThrowableObject* FindNearestItem() const;
	//アイテムを拾った時の処理
	void PickUp(AThrowableObject* Target);
	//InputManagerを初期化する関数
	void InitializeInputManager();
	//アイテムを投げた時の処理
	void Throw();

protected:

	//カメラを制御するアクター
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<AActor> Player = nullptr;

private:
	//拾える範囲にあるアイテムの候補リスト
	UPROPERTY(Transient)
	TArray<TObjectPtr<AThrowableObject>> NearbyItems;

	//現在持っているアイテム
	UPROPERTY(Transient)
	TObjectPtr<AThrowableObject> HeldItem = nullptr;

	//入力管理を行うオブジェクト
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;

};
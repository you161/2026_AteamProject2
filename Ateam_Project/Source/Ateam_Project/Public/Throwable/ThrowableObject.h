#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableState.h"
#include "ThrowableObject.generated.h"

class USphereComponent;
class UThrowableObjectData;
class AThrowableManager;
class APlayerPawn;

/**
 * 投げることができるアイテム。
 */
UCLASS()
class ATEAM_PROJECT_API AThrowableObject : public AActor
{
	GENERATED_BODY()

public:
	AThrowableObject();

	virtual void Tick(float DeltaTime) override;

	//生成時にマネージャーを注入する
	void SetThrowableManager(AThrowableManager* InManager);
	//持たれた時の処理
	void OnPickedUp(AActor* Holder);
	//回収数に合わせて持っている間のスケールを更新する
	void UpdateHeldScale(int32 CollectedCount);
	//投げられた時の処理
	void OnThrown(const FVector& Direction, int32 StackCount);
	//持たれている状態かどうかを返す
	bool IsHeld() const { return CurrentState == EThrowableState::Held; }

protected:
	virtual void BeginPlay() override;

	//他のActorと重なった時に呼ばれる
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	//指定方向に飛んでいく処理
	void UpdateThrownMovement(float DeltaTime);
	//持たれている間の回転処理
	void UpdateHeldRotation(float DeltaTime);
	//アイテムの大きさに合わせて持つ位置を更新する
	void UpdateHeldLocation();

protected:
	//当たり判定
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision = nullptr;
	//投げれるオブジェクトを管理しているアクタ
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableObject|ThrowableManager")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;
	//投げれるオブジェクトのデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThrowableObject|Data")
	TObjectPtr<UThrowableObjectData> ThrowableData = nullptr;

private:
	//現在の状態
	EThrowableState CurrentState = EThrowableState::Pickable;
	//持っているプレイヤー
	TWeakObjectPtr<APlayerPawn> HolderPawn = nullptr;
	//飛んでいく方向
	FVector CurrentDirection = FVector::ZeroVector;
	//投げ始めの座標
	FVector ThrowStartLocation = FVector::ZeroVector;
	//生成時のスケール
	FVector BaseScale = FVector::OneVector;
	//投げられた時の個数
	int32 ThrowStackCount = 0;
};
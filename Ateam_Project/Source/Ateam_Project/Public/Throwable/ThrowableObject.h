
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableState.h"
#include "ThrowableObject.generated.h"

class USphereComponent;
class UThrowableObjectData;
class AThrowableManager;
class AHealthManager;
class APlayerPawn;

/**
 *投げることができるアイテム
 */
UCLASS()
class ATEAM_PROJECT_API AThrowableObject : public AActor
{
	GENERATED_BODY()
	
public:	

	AThrowableObject();

protected:

	virtual void BeginPlay() override;

	//他のActorと重なった時に呼ばれる関数
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:	

	virtual void Tick(float DeltaTime) override;
	//持たれた時の関数
	void OnPickedUp(AActor* Holder);
	//投げられた時の関数
	void OnThrown(const FVector& Direction);
	//現在の状態を取得する関数
	bool IsHeld() const { return CurrentState == EThrowableState::Held; }

private:
	//指定方向に飛んでいく処理
	void UpdateThrownMovement(float DeltaTime);
	//持たれている間の回転処理
	void UpdateHeldRotation(float DeltaTime);
	//アイテムの大きさに合わせて持つ位置を更新する
	void UpdateHeldLocation();

protected:
	//当たり判定用
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision = nullptr;
	//投げれるオブジェクトを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableObject|ThrowableManager")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;
	//HP
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableObject|HealthManager")
	TObjectPtr<AHealthManager> HealthManager = nullptr;
	//プレイヤー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableObject|PlayerPawn")
	TObjectPtr<APlayerPawn> PlayerPawn = nullptr;
	//投げれるオブジェクトのデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThrowableObject|Data")
	TObjectPtr<UThrowableObjectData> ThrowableData = nullptr;

private:
	//現在の状態を管理するステート
	EThrowableState CurrentState = EThrowableState::Pickable;
	//飛んでいく方向を保存する変数
	FVector CurrentDirection = FVector::ZeroVector;
	//投げ始めの座標を保存する変数
	FVector ThrowStartLocation = FVector::ZeroVector;
	//投げられた時のスケールを保存する変数
	FVector BaseScale = FVector::ZeroVector;
	//回収されたかどうかのフラグ
	bool bIsCollected = false;
};
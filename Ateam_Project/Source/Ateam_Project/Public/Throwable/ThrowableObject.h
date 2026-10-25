
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableState.h"
#include "ThrowableObject.generated.h"

class USphereComponent;
class AThrowableManager;

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

	//他のActorとの重なりが終わった時に呼ばれる関数
	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:	

	virtual void Tick(float DeltaTime) override;
	//持たれた時の関数
	void OnPickedUp(AActor* Holder);
	//投げられた時の関数
	void OnThrown(const FVector& Direction);

private:
	//指定方向に飛んでいく処理
	void UpdateThrownMovement(float DeltaTime);

protected:
	//当たり判定用
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;
	//投げれるオブジェクトを管理するマネージャー
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	float MoveSpeed = 500.0f;

	//投げてから消えるまでの飛距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	float MaxThrowDistance = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	FVector Offset = FVector(100.0f, 0.0f, 50.0f);

private:
	//現在の状態を管理するステート
	EThrowableState CurrentState = EThrowableState::Pickable;
	//飛んでいく方向を保存する変数
	FVector CurrentDirection = FVector::ZeroVector;
	//投げ始めの座標をほs￥損する変数
	FVector ThrowStartLocation = FVector::ZeroVector;
};
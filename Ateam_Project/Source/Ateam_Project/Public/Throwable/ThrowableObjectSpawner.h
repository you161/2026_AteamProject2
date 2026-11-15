#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableObjectSpawner.generated.h"

class AThrowableObject;
class AThrowableManager;

/**
 * 投擲オブジェクトを一定間隔で生成するクラス。
 */
UCLASS()
class ATEAM_PROJECT_API AThrowableObjectSpawner : public AActor
{
	GENERATED_BODY()

public:
	AThrowableObjectSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	//生成処理
	void SpawnObject();
	//破棄されたオブジェクトを管理リストから除く
	void RemoveDestroyedObjects();

protected:
	//生成するクラス
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AThrowableObject> ObjectClass = nullptr;
	//生成したオブジェクトに渡すマネージャー
	UPROPERTY(EditInstanceOnly, Category = "Spawn")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;
	//生成間隔
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0.1"))
	float SpawnInterval = 3.0f;
	//最初の生成までの待ち時間
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0.0"))
	float InitialDelay = 1.0f;
	//同時に存在できる最大数、0は無制限
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0"))
	int32 MaxSpawnCount = 5;
	//生成位置のオフセット
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector SpawnOffset = FVector::ZeroVector;
	//生成座標
	UPROPERTY(EditInstanceOnly, Category = "Spawn")
	TArray<TObjectPtr<AActor>> SpawnLocations = {};

private:
	//生成タイマー
	FTimerHandle SpawnTimerHandle;
	//生成したオブジェクトと、使用したスポーン位置の情報
	TMap<TWeakObjectPtr<AThrowableObject>, int32> SpawnedObjectPointIndices = {};
};
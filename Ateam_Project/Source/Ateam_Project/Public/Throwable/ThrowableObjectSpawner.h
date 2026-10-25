
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableObjectSpawner.generated.h"

class AThrowableObject;

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

	//生成処理（タイマーから呼ばれる）
	void SpawnObject();

protected:

	//レベルに配置してある生成元オブジェクト
	UPROPERTY(EditInstanceOnly, Category = "Spawn")
	TObjectPtr<AThrowableObject> SourceObject = nullptr;
	//生成間隔
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0.1"))
	float SpawnInterval = 3.0f;
	//最初の生成までの待ち時間
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0.0"))
	float InitialDelay = 1.0f;
	//同時に存在できる最大数（0は無制限）
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0"))
	int32 MaxSpawnCount = 5;
	//生成位置のオフセット
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector SpawnOffset = FVector::ZeroVector;
	//生成座標
	UPROPERTY(EditInstanceOnly, Category = "Spawn")
	TArray<TObjectPtr<AActor>> SpawnLocations = {};

private:

	FTimerHandle SpawnTimerHandle;

	//生成したオブジェクトの管理リスト
	UPROPERTY(Transient)
	TArray<TObjectPtr<AThrowableObject>> SpawnedObjects = {};
	//生成したオブジェクトと、使用したスポーン位置の管理リスト
	UPROPERTY()
	TMap<TObjectPtr<AThrowableObject>, int32> SpawnedObjectPointIndices;

};
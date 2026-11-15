#include "Throwable/ThrowableObjectSpawner.h"
#include "Throwable/ThrowableObject.h"
#include "Throwable/ThrowableManager.h"
#include "TimerManager.h"
#include "Engine/World.h"

AThrowableObjectSpawner::AThrowableObjectSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AThrowableObjectSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!ObjectClass)
	{
		UE_LOG(LogThrowable, Error, TEXT("[%s] ObjectClass is not set."), *GetName());
		return;
	}

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogThrowable, Error, TEXT("[%s] ThrowableManager is not set."), *GetName());
		return;
	}

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AThrowableObjectSpawner::SpawnObject,
		SpawnInterval,
		true,
		InitialDelay
	);
}

void AThrowableObjectSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	Super::EndPlay(EndPlayReason);
}

//投擲オブジェクトを生成する
void AThrowableObjectSpawner::SpawnObject()
{
	RemoveDestroyedObjects();

	//上限チェック
	if (MaxSpawnCount > 0 && SpawnedObjectPointIndices.Num() >= MaxSpawnCount)
	{
		return;
	}

	//使用中のスポーン位置
	TSet<int32> UsedIndices = {};
	for (const TPair<TWeakObjectPtr<AThrowableObject>, int32>& Pair : SpawnedObjectPointIndices)
	{
		UsedIndices.Add(Pair.Value);
	}

	//空いているスポーン位置
	TArray<int32> AvailableIndices = {};
	for (int32 Index = 0; Index < SpawnLocations.Num(); ++Index)
	{
		if (IsValid(SpawnLocations[Index]) && !UsedIndices.Contains(Index))
		{
			AvailableIndices.Add(Index);
		}
	}

	//空いている位置がなければ生成しない
	if (AvailableIndices.IsEmpty())
	{
		return;
	}

	//ランダムで生成位置を決定
	const int32 SelectedIndex = AvailableIndices[FMath::RandRange(0, AvailableIndices.Num() - 1)];
	const FVector SpawnLocation = SpawnLocations[SelectedIndex]->GetActorLocation() + SpawnOffset;
	const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

	AThrowableObject* SpawnedObject = GetWorld()->SpawnActorDeferred<AThrowableObject>(
		ObjectClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!IsValid(SpawnedObject))
	{
		UE_LOG(LogThrowable, Error, TEXT("[%s] Failed to spawn throwable object."), *GetName());
		return;
	}

	SpawnedObject->SetThrowableManager(ThrowableManager);
	SpawnedObject->FinishSpawning(SpawnTransform);

	SpawnedObjectPointIndices.Add(SpawnedObject, SelectedIndex);
}

//破棄されたオブジェクトを管理リストから除く
void AThrowableObjectSpawner::RemoveDestroyedObjects()
{
	for (auto It = SpawnedObjectPointIndices.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			It.RemoveCurrent();
		}
	}
}
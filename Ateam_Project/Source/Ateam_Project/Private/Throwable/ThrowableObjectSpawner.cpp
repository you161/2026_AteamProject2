
#include "Throwable/ThrowableObjectSpawner.h"
#include "Throwable/ThrowableObject.h"
#include "TimerManager.h"
#include "Engine/World.h"

AThrowableObjectSpawner::AThrowableObjectSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AThrowableObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(SourceObject))
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] SourceObject is not set."), *GetName());
		return;
	}

	SourceObject->SetActorHiddenInGame(true);
	SourceObject->SetActorEnableCollision(false);

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

//投擲オブジェクトを生成する。
void AThrowableObjectSpawner::SpawnObject()
{
	if (!IsValid(SourceObject))
	{
		return;
	}

	//破棄されたオブジェクトを除く
	for (auto It = SpawnedObjectPointIndices.CreateIterator(); It; ++It)
	{
		if (!IsValid(It.Key()))
		{
			It.RemoveCurrent();
		}
	}

	//上限チェック
	if (MaxSpawnCount > 0 && SpawnedObjectPointIndices.Num() >= MaxSpawnCount)
	{
		return;
	}

	//生成されているSpanwPointの情報
	TSet<int32> UsedIndices = {};
	for (const TPair<TObjectPtr<AThrowableObject>, int32>& Pair : SpawnedObjectPointIndices)
	{
		UsedIndices.Add(Pair.Value);
	}

	//生成されていないSpawnPointの情報
	TArray<int32> AvailableIndices = {};
	for (int32 Index = 0; Index < SpawnLocations.Num(); ++Index)
	{
		if (IsValid(SpawnLocations[Index]) && !UsedIndices.Contains(Index))
		{
			AvailableIndices.Add(Index);
		}
	}

	//空いている位置がなければ生成しない。
	if (AvailableIndices.IsEmpty())
	{
		return;
	}

	//ランダムで生成位置を決定
	const int32 SelectedIndex = AvailableIndices[FMath::RandRange(0, AvailableIndices.Num() - 1)];
	const FVector SpawnLocation = SpawnLocations[SelectedIndex]->GetActorLocation();

	//オブジェクトの生成
	FActorSpawnParameters Params;
	Params.Template = SourceObject;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AThrowableObject* NewObject = GetWorld()->SpawnActor<AThrowableObject>(
		SourceObject->GetClass(),
		SpawnLocation,
		GetActorRotation(),
		Params
	);

	if (IsValid(NewObject))
	{
		NewObject->SetActorLocation(SpawnLocation);
		NewObject->SetActorHiddenInGame(false);
		NewObject->SetActorEnableCollision(true);

		SpawnedObjectPointIndices.Add(NewObject, SelectedIndex);
	}
}
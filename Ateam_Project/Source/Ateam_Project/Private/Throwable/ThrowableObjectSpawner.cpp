
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

void AThrowableObjectSpawner::SpawnObject()
{
	if (!IsValid(SourceObject))
	{
		return;
	}

	//破棄されたオブジェクトをリストから除く
	SpawnedObjects.RemoveAll([](const TObjectPtr<AThrowableObject>& Obj)
		{
			return !IsValid(Obj);
		});

	//上限チェック
	if (MaxSpawnCount > 0 && SpawnedObjects.Num() >= MaxSpawnCount)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Template = SourceObject;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector SpawnLocation = GetActorLocation() + SpawnOffset;

	AThrowableObject* NewObj = GetWorld()->SpawnActor<AThrowableObject>(
		SourceObject->GetClass(),
		SpawnLocation,
		GetActorRotation(),
		Params
	);

	if (IsValid(NewObj))
	{
		NewObj->SetActorHiddenInGame(false);
		NewObj->SetActorEnableCollision(true);

		SpawnedObjects.Add(NewObj);
	}
}
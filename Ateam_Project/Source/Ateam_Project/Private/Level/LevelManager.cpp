
#include "Level/LevelManager.h"
#include "Kismet/GameplayStatics.h"

ALevelManager::ALevelManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();	
}

void ALevelManager::LoadTitleLevel()
{
	if (TitleLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("TitleLevel is null"));
		return;
	}

	LoadLevel(TitleLevel);
}

void ALevelManager::LoadMovieLevel()
{
	if (MovieLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("MovieLevel is null"));
		return;
	}

	LoadLevel(MovieLevel);
}

void ALevelManager::LoadMainLevel()
{
	if (MainLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("MainLevel is null"));
		return;
	}

	LoadLevel(MainLevel);
}

void ALevelManager::LoadClearLevel()
{
	if (ClearLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("ClearLevel is null"));
		return;
	}

	LoadLevel(ClearLevel);
}

void ALevelManager::LoadGameOverLevel()
{
	if (GameOverLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("GameOverLevel is null"));
		return;
	}

	LoadLevel(GameOverLevel);
}

void ALevelManager::LoadLevel(TSoftObjectPtr<UWorld> NextLevel)const
{
	if (!IsValid(GetWorld()))
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld is null"));
		return;
	}

	if (NextLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("NextLevel is null"));
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel);
}
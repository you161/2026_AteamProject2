
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
	if (!TitleLevel.IsValid())
	{
		return;
	}

	LoadLevel(TitleLevel);
}

void ALevelManager::LoadMovieLevel()
{
	if (!MovieLevel.IsValid())
	{
		return;
	}

	LoadLevel(MovieLevel);
}

void ALevelManager::LoadMainLevel()
{
	if (!MainLevel.IsValid())
	{
		return;
	}

	LoadLevel(MainLevel);
}

void ALevelManager::LoadClearLevel()
{
	if (!ClearLevel.IsValid())
	{
		return;
	}

	LoadLevel(ClearLevel);
}

void ALevelManager::LoadGameOverLevel()
{
	if (!GameOverLevel.IsValid())
	{
		return;
	}

	LoadLevel(GameOverLevel);
}

void ALevelManager::LoadLevel(TSoftObjectPtr<UWorld> NextLevel)const
{
	if (!IsValid(GetWorld()))
	{
		return;
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel);
}
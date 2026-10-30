
#include "GameManager/GameJudge.h"
#include "HitPointBar.h"
#include "Level/LevelManager.h"

AGameJudge::AGameJudge()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGameJudge::BeginPlay()
{
	Super::BeginPlay();

	if(!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager is null in AGameJudge::BeginPlay"));
	}
}

void AGameJudge::GameClear()const
{
	if (IsValid(LevelManager))
	{
		LevelManager->LoadClearLevel();
	}
}

void AGameJudge::GameOver()const
{
	if (IsValid(LevelManager))
	{
		LevelManager->LoadGameOverLevel();
	}
}
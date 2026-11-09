
#include "Player/PlayerLevelManager.h"
#include "Player/PlayerLevelData.h"
#include "Throwable/ThrowableManager.h"

APlayerLevelManager::APlayerLevelManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerLevelManager::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel = 1;
	LevelGage = 0.0f;
}

void APlayerLevelManager::AddExp()
{
	if (!IsValid(PlayerLevelData))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerLevelData is null in APlayerLevelManager::AddExp"));
		return;
	}

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogTemp, Error, TEXT("ThrowableManager is null in APlayerLevelManager::AddExp"));
		return;
	}

	//オブジェクトの個数取得
	int32 Index = FMath::Clamp(ThrowableManager->GetThrownCount() , 0, PlayerLevelData->GaugeGainTable.Num() - 1);
	
	//経験値を追加
	LevelGage += PlayerLevelData->GaugeGainTable[Index];

	//レベルアップの判定
	if (LevelGage >= PlayerLevelData->MaxGaugeTable[CurrentLevel])
	{
		//レベルアップ処理
		if (CurrentLevel + 1 < PlayerLevelData->MaxGaugeTable.Num())
		{
			LevelGage = LevelGage - PlayerLevelData->MaxGaugeTable[CurrentLevel];
			CurrentLevel++;
			UE_LOG(LogTemp, Log, TEXT("レベルアップ！ 現在のレベル: %d"), CurrentLevel);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("最大レベルに到達しました。現在のレベル: %d"), CurrentLevel);
			LevelGage = PlayerLevelData->MaxGaugeTable[CurrentLevel];
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("経験値を追加しました。現在の経験値: %.2f"), LevelGage);
	}
}
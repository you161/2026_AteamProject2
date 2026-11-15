
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

	const TArray<float>& GainTable = PlayerLevelData->GaugeGainTable;
	const TArray<float>& MaxTable = PlayerLevelData->MaxGaugeTable;

	if (GainTable.Num() == 0 || MaxTable.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Level table is empty in APlayerLevelManager::AddExp"));
		return;
	}

	//現在のレベルがテーブルの範囲内かを確認する
	if (!MaxTable.IsValidIndex(CurrentLevel))
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentLevel is out of range of MaxGaugeTable"));
		return;
	}

	//オブジェクトの個数取得
	const int32 Index = FMath::Clamp(ThrowableManager->GetThrownCount(), 0, GainTable.Num() - 1);

	//経験値を追加
	LevelGage += GainTable[Index];

	//レベルアップの判定
	if (LevelGage >= MaxTable[CurrentLevel])
	{
		//次のレベルが存在すればレベルアップ
		if (MaxTable.IsValidIndex(CurrentLevel + 1))
		{
			LevelGage -= MaxTable[CurrentLevel];
			CurrentLevel++;
			UE_LOG(LogTemp, Log, TEXT("レベルアップ！ 現在のレベル: %d"), CurrentLevel);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("最大レベルに到達しました。現在のレベル: %d"), CurrentLevel);
			LevelGage = MaxTable[CurrentLevel];
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("経験値を追加しました。現在の経験値: %.2f"), LevelGage);
	}
}
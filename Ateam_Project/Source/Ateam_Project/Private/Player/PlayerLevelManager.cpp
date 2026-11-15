#include "Player/PlayerLevelManager.h"
#include "Player/PlayerLevelData.h"
#include "Throwable/ThrowableManager.h"

DEFINE_LOG_CATEGORY(LogPlayerLevel);

APlayerLevelManager::APlayerLevelManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerLevelManager::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel = 1;
	LevelGauge = 0.0f;

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogPlayerLevel, Error, TEXT("ThrowableManager is null in APlayerLevelManager::BeginPlay"));
		return;
	}

	ThrowableManager->OnHitEnemy.AddUObject(this, &APlayerLevelManager::AddExp);
}

void APlayerLevelManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(ThrowableManager))
	{
		ThrowableManager->OnHitEnemy.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

//経験値を追加する
void APlayerLevelManager::AddExp(int32 StackCount)
{
	if (!IsValid(PlayerLevelData))
	{
		UE_LOG(LogPlayerLevel, Error, TEXT("PlayerLevelData is null in APlayerLevelManager::AddExp"));
		return;
	}

	const TArray<float>& GainTable = PlayerLevelData->GaugeGainTable;
	const TArray<float>& MaxTable = PlayerLevelData->MaxGaugeTable;

	if (GainTable.IsEmpty() || MaxTable.IsEmpty())
	{
		UE_LOG(LogPlayerLevel, Error, TEXT("Level table is empty in APlayerLevelManager::AddExp"));
		return;
	}

	//現在のレベルがテーブルの範囲内かを確認する
	if (!MaxTable.IsValidIndex(CurrentLevel))
	{
		UE_LOG(LogPlayerLevel, Error, TEXT("CurrentLevel is out of range of MaxGaugeTable"));
		return;
	}

	//投げた時の個数で獲得量を決定する
	const int32 GainIndex = FMath::Clamp(StackCount, 0, GainTable.Num() - 1);
	LevelGauge += GainTable[GainIndex];

	//レベルアップの判定
	while (LevelGauge >= MaxTable[CurrentLevel])
	{
		//最大レベルならゲージを上限で止める
		if (!MaxTable.IsValidIndex(CurrentLevel + 1))
		{
			LevelGauge = MaxTable[CurrentLevel];
			UE_LOG(LogPlayerLevel, Log, TEXT("最大レベルに到達しました。現在のレベル: %d"), CurrentLevel);
			return;
		}

		LevelGauge -= MaxTable[CurrentLevel];
		CurrentLevel++;
		UE_LOG(LogPlayerLevel, Log, TEXT("レベルアップ！ 現在のレベル: %d"), CurrentLevel);
	}

	UE_LOG(LogPlayerLevel, Log, TEXT("経験値を追加しました。現在の経験値: %.2f"), LevelGauge);
}
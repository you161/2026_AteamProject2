#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerLevelManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerLevel, Log, All);

class UPlayerLevelData;
class AThrowableManager;

/**
 * プレイヤーのレベルと経験値を管理するクラス。
 */
UCLASS()
class ATEAM_PROJECT_API APlayerLevelManager : public AActor
{
	GENERATED_BODY()

public:
	APlayerLevelManager();

	//経験値を追加する
	void AddExp(int32 StackCount);
	//現在のレベルを取得する
	int32 GetLevel() const { return CurrentLevel; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	//プレイヤーのレベルデータを格納するデータアセット
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerLevelManager|Data")
	TObjectPtr<UPlayerLevelData> PlayerLevelData = nullptr;
	//敵に当たった通知を受け取るマネージャー
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerLevelManager|Throwable")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;

private:
	//現在のレベル
	int32 CurrentLevel = 1;
	//現在の経験値ゲージ
	float LevelGauge = 0.0f;
};
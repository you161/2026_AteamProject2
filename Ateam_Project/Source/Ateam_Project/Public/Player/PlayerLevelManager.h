
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerLevelManager.generated.h"

class UPlayerLevelData;
class AThrowableManager;

UCLASS()
class ATEAM_PROJECT_API APlayerLevelManager : public AActor
{
	GENERATED_BODY()
	
public:	

	APlayerLevelManager();

protected:

	virtual void BeginPlay() override;

public:	
	//経験値を追加する関数
	void AddExp();
	//現在のレベルを取得する関数
	int32 GetLevel() const { return CurrentLevel; }

protected:

	//プレイヤーのレベルデータを格納するデータアセット
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerLevelManager|Data")
	TObjectPtr<UPlayerLevelData> PlayerLevelData = nullptr;

	//拾ったオブジェクトを管理するマネージャー
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthManager|Throwable")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;

private:
	int32 CurrentLevel = 1; //現在のレベルを保持する変数
	float LevelGage = 0.0f; //現在の経験値を保持する変数
};
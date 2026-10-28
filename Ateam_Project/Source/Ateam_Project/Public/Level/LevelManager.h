
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class ATEAM_PROJECT_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelManager();
protected:
	virtual void BeginPlay() override;
public:	
	//タイトルシーンの読み込み
	void LoadTitleLevel();
	//動画シーンの読み込み
	void LoadMovieLevel();
	//メインシーン読み込み
	void LoadMainLevel();
	//クリアシーンの読み込み
	void LoadClearLevel();
	//ゲームオーバーシーンの読み込み
	void LoadGameOverLevel();
private:
	//レベル読み込み
	void LoadLevel(TSoftObjectPtr<UWorld> NextLevel) const;
protected:
	//タイトルシーン
	UPROPERTY(EditAnywhere, Category = "Level|Title")
	TSoftObjectPtr<UWorld> TitleLevel = nullptr;
	//動画再生シーン
	UPROPERTY(EditAnywhere, Category = "Level|Movie")
	TSoftObjectPtr<UWorld> MovieLevel = nullptr;
	//メインシーン
	UPROPERTY(EditAnywhere, Category = "Level|Main")
	TSoftObjectPtr<UWorld> MainLevel = nullptr;
	//クリアシーン
	UPROPERTY(EditAnywhere, Category = "Level|Clear")
	TSoftObjectPtr<UWorld> ClearLevel = nullptr;
	//ゲームオーバーシーン
	UPROPERTY(EditAnywhere, Category = "Level|GameOver")
	TSoftObjectPtr<UWorld> GameOverLevel = nullptr;
};
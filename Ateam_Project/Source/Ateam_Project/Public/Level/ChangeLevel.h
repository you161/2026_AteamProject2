// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "ChangeLevel.generated.h"

class UInputAction;
class UInputMappingContext;
class ALevelManager;

UENUM(BlueprintType)
enum class LevelSeed : uint8 {
	None = 0,
	TitleLevel,
	MovieLevel,
	MainLevel,
	ClearLevel,
	GameOverLevel
};

/**
 * レベル遷移
 */
UCLASS()
class ATEAM_PROJECT_API AChangeLevel : public APawn
{
	GENERATED_BODY()
	
public:
	AChangeLevel();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// レベル遷移入力
	void ChangeLevel();

	// LevelManager
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelManager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALevelManager> LevelManagerClass = nullptr;

	// レベルの種類
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelManager", meta = (AllowPrivateAccess = "true"))
	LevelSeed LevelName = LevelSeed::None;

	// 入力マッピングコンテキスト
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	// レベル遷移入力
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ChangeLevelAction;

	TObjectPtr<ALevelManager> LevelManager = nullptr;
};

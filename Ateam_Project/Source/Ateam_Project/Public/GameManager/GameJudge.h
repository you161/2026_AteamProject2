
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameJudge.generated.h"

class UHitPointBar;
class ALevelManager;

UCLASS()
class ATEAM_PROJECT_API AGameJudge : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameJudge();

	void GameClear()const;
	void GameOver()const;

protected:

	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelManager")
	TObjectPtr<ALevelManager> LevelManager = nullptr;
};
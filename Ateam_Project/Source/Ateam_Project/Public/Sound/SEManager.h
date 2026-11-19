
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundData.h"
#include "SEManager.generated.h"

UCLASS()
class ATEAM_PROJECT_API ASEManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASEManager();

protected:

	virtual void BeginPlay() override;

public:	
	//SE再生
	void PlaySE(ESEType Type);
	//SE停止
	void StopSE(ESEType Type);

protected:
	//サウンドデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundData")
	TObjectPtr<USoundData> SoundData = nullptr;
	//ループ用
	UPROPERTY()
	TMap<ESEType, TObjectPtr<UAudioComponent>> LoopingSEData;
};
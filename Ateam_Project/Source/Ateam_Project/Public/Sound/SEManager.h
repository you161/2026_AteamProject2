
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
	//敵
	void PlaySE(ESEType Type);

protected:
	//サウンドデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundData")
	TObjectPtr<USoundData> SoundData = nullptr;
};
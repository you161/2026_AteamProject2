
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundData.h"
#include "BGMManager.generated.h"

class UAudioComponent;

UCLASS()
class ATEAM_PROJECT_API ABGMManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABGMManager();

protected:
	virtual void BeginPlay() override;

private:	
	void PlayBGM();
	void StopBGM(float FadeOutTime);
protected:
	//サウンドデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundData")
	TObjectPtr<USoundData> SoundData = nullptr;
	//BGMの種類
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGMType")
	EBGMType BGMType = EBGMType::None;

private:
	//オーディオコンポーネント
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;
	//BGMが設定されているか
	bool bIsSet = false;
};
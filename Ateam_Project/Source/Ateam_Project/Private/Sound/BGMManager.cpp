
#include "Sound/BGMManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ABGMManager::ABGMManager()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
}

void ABGMManager::BeginPlay()
{
	Super::BeginPlay();	

	if (!IsValid(SoundData))
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundDataが設定されていない"));
	}

	bIsSet = false;
	PlayBGM();
}

//BGMを再生する関数
void ABGMManager::PlayBGM()
{
	const FSoundEntry* Entry = SoundData ? SoundData->BGMMap.Find(BGMType) : nullptr;

	if (!Entry || !Entry->Sound)
	{
		return;
	}

	StopBGM(0.0f);

	AudioComponent = UGameplayStatics::SpawnSound2D
	(
		GetWorld(),
		Entry->Sound,
		Entry->Volume,
		Entry->Pitch
	);
}

//BGMを停止する関数
void ABGMManager::StopBGM(float FadeOutTime)
{
	if (IsValid(AudioComponent))
	{
		AudioComponent->FadeOut(FadeOutTime, 0.0f);
		AudioComponent = nullptr;
	}
}
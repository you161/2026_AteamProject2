
#include "Sound/SEManager.h"
#include "Kismet/GameplayStatics.h"

ASEManager::ASEManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASEManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(SoundData))
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundDataが設定されていない"));
	}
}

//SEを再生する関数
void ASEManager::PlaySE(ESEType Type)
{
    if (!SoundData)
    {
        return;
    }

    const FSoundEntry* Entry = SoundData->SEMap.Find(Type);

    if (!Entry || !Entry->Sound)
    {
        UE_LOG(LogTemp, Warning, TEXT("SEが未設定: %d"), static_cast<int32>(Type));
        return;
    }

    UGameplayStatics::PlaySound2D(GetWorld(), Entry->Sound, Entry->Volume, Entry->Pitch);
}
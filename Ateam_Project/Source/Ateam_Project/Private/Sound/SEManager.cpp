
#include "Sound/SEManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
    if (!SoundData) return;

    const FSoundEntry* Entry = SoundData->SEMap.Find(Type);
    if (!Entry || !Entry->Sound)
    {
        UE_LOG(LogTemp, Warning, TEXT("SEが未設定: %d"), static_cast<int32>(Type));
        return;
    }

    if (Entry->bLoop)
    {
        //すでにループ再生中なら二重に鳴らさない
        if (TObjectPtr<UAudioComponent>* Existing = LoopingSEData.Find(Type))
        {
            if (IsValid(*Existing) && (*Existing)->IsPlaying())
            {
                return;
            }
        }

        UAudioComponent* Comp = UGameplayStatics::SpawnSound2D(
            GetWorld(),
            Entry->Sound,
            Entry->Volume,
            Entry->Pitch
        );

        if (Comp)
        {
            LoopingSEData.Add(Type, Comp);
        }
    }
    else
    {
        UGameplayStatics::PlaySound2D(GetWorld(), Entry->Sound, Entry->Volume, Entry->Pitch);
    }
}

//SEを止める
void ASEManager::StopSE(ESEType Type)
{
    if (TObjectPtr<UAudioComponent>* Comp = LoopingSEData.Find(Type))
    {
        if (IsValid(*Comp))
        {
            (*Comp)->Stop();
        }
        LoopingSEData.Remove(Type);
    }
}
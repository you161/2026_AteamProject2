
#include "Throwable/ThrowableManager.h"
#include "Throwable/ThrowableObject.h"
#include "Input/InputManager.h"
#include "Input/InputReceiver.h"

AThrowableManager::AThrowableManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThrowableManager::BeginPlay() 
{
	Super::BeginPlay();

	InitializeInputManager();
}

void AThrowableManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(InputManager))
	{
		return;
	}

	if (InputManager->ConsumeInteract()) {

		if (IsValid(HeldItem))
		{
			Throw();
		}
	}
}

//現在持っているアイテムを設定する
void AThrowableManager::PickUp(AThrowableObject* Target)
{
	if (!IsValid(Target)) 
	{
		return;
	}

	if (CollectedCount >= MaxObjectCount)
	{
		return;
	}

	if (!IsValid(HeldItem))
	{
		HeldItem = Target;
		CollectedCount = 0;
		HeldItem->OnPickedUp(Player);
	}
	else
	{
		CollectedCount++;
		HeldItem->OnPickedUp(Player);
		Target->Destroy();
	}
}

//アイテムを投げる処理
void AThrowableManager::Throw()
{
	HeldItem->OnThrown(Player->GetActorForwardVector());
	HeldItem = nullptr;
	CollectedCount = 0;
}

//InputManagerを初期化する関数
void AThrowableManager::InitializeInputManager()
{
	if (AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetWorld()->GetFirstPlayerController()))
	{
		InputManager = InputReceiver->GetInputManager();
	}
}
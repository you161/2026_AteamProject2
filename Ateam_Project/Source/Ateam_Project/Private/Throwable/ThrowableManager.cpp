#include "Throwable/ThrowableManager.h"
#include "Throwable/ThrowableObject.h"
#include "Input/InputManager.h"
#include "Input/InputReceiver.h"
#include "Player/PlayerDamageData.h"

DEFINE_LOG_CATEGORY(LogThrowable);

AThrowableManager::AThrowableManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AThrowableManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeInputManager();
}

void AThrowableManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(InputManager))
	{
		InputManager->OnInteract.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

//アイテムを拾えたらtrueを返す
bool AThrowableManager::PickUp(AThrowableObject* Target)
{
	if (!IsValid(Target))
	{
		return false;
	}

	if (!IsValid(PlayerDamageData) || !IsValid(Player))
	{
		UE_LOG(LogThrowable, Error, TEXT("PlayerDamageData or Player is null in AThrowableManager::PickUp"));
		return false;
	}

	//所持上限
	const int32 MaxCollectCount = FMath::Max(0, PlayerDamageData->PlayerAttackDamage.Num() - 1);

	if (CollectedCount >= MaxCollectCount)
	{
		return false;
	}

	if (!IsValid(HeldItem))
	{
		HeldItem = Target;
		HeldItem->OnPickedUp(Player);
	}
	else
	{
		Target->Destroy();
	}

	++CollectedCount;
	HeldItem->UpdateHeldScale(CollectedCount);
	return true;
}

//投げたアイテムが敵に当たったことを通知する
void AThrowableManager::NotifyHitEnemy(int32 StackCount)
{
	OnHitEnemy.Broadcast(StackCount);
}

//InputManagerを取得して投擲入力を購読する
void AThrowableManager::InitializeInputManager()
{
	AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetWorld()->GetFirstPlayerController());

	if (!IsValid(InputReceiver))
	{
		UE_LOG(LogThrowable, Error, TEXT("InputReceiver is not found in AThrowableManager::InitializeInputManager"));
		return;
	}

	InputManager = InputReceiver->GetInputManager();

	if (!IsValid(InputManager))
	{
		UE_LOG(LogThrowable, Error, TEXT("InputManager is null in AThrowableManager::InitializeInputManager"));
		return;
	}

	InputManager->OnInteract.AddUObject(this, &AThrowableManager::HandleInteract);
}

//投擲入力が来た時の処理
void AThrowableManager::HandleInteract()
{
	if (IsValid(HeldItem))
	{
		Throw();
	}
}

//持っているアイテムを投げる
void AThrowableManager::Throw()
{
	if (!IsValid(HeldItem) || !IsValid(Player))
	{
		return;
	}

	//投げた時の個数はアイテム自身に持たせる
	HeldItem->OnThrown(Player->GetActorForwardVector(), CollectedCount);
	HeldItem = nullptr;
	CollectedCount = 0;
}
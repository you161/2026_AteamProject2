
#include "Throwable/ThrowableManager.h"
#include "Throwable/ThrowableObject.h"
#include "Input/InputManager.h"
#include "Input/InputReceiver.h"
#include "Player/PlayerPawn.h"

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
			PlayerPawn->SetHasObject(false);
		}
		else
		{
			AThrowableObject* Target = FindNearestItem();
			if (IsValid(Target))
			{
				PickUp(Target);
				PlayerPawn->SetHasObject(true);
			}
		}
	}
}

//近くのアイテムの候補リストに追加する
void AThrowableManager::AddCandidate(AThrowableObject* Item)
{
	if (IsValid(Item))
	{
		NearbyItems.AddUnique(Item);
	}
}

//近くのアイテムの候補リストから削除する
void AThrowableManager::RemoveCandidate(AThrowableObject* Item)
{
	if (IsValid(Item)) 
	{
		NearbyItems.Remove(Item);
	}
}

//近くのアイテムの中から一番近いものを返す
AThrowableObject* AThrowableManager::FindNearestItem() const
{
	if (NearbyItems.IsEmpty() || !IsValid(Player))
	{
		return nullptr;
	}

	AThrowableObject* NearestItem = NearbyItems[0];

	for (int i = 1; i < NearbyItems.Num(); ++i)
	{
		float NearestDistance = FVector::Dist(NearestItem->GetActorLocation(), Player->GetActorLocation());
		float CurrentDistance = FVector::Dist(NearbyItems[i]->GetActorLocation(), Player->GetActorLocation());

		if (CurrentDistance < NearestDistance)
		{
			NearestItem = NearbyItems[i];
		}
	}

	return NearestItem;
}

//現在持っているアイテムを設定する
void AThrowableManager::PickUp(AThrowableObject* Target)
{
	HeldItem = Target;
	NearbyItems.Remove(Target);
	HeldItem->OnPickedUp(Player);
}

void AThrowableManager::Throw()
{
	HeldItem->OnThrown(Player->GetActorForwardVector());
	HeldItem = nullptr;
}

void AThrowableManager::InitializeInputManager()
{
	if (AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetWorld()->GetFirstPlayerController()))
	{
		InputManager = InputReceiver->GetInputManager();
	}
}
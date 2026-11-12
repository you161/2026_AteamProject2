
#include "Player/StaminaManager.h"

AStaminaManager::AStaminaManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStaminaManager::BeginPlay()
{
	Super::BeginPlay();
	bCanSprint = true;
	Stamina = MaxStamina;
}

void AStaminaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Sprint(DeltaTime);

	//値が変わったときだけ通知する
	if (!FMath::IsNearlyEqual(Stamina, LastNotifiedStamina) || bCanSprint != bLastNotifiedCanSprint)
	{
		LastNotifiedStamina = Stamina;
		bLastNotifiedCanSprint = bCanSprint;
		OnStaminaChanged.Broadcast(GetStaminaRatio(), bCanSprint);
	}
}

//スタミナの減少と回復を制御する関数
void AStaminaManager::Sprint(float DeltaTime)
{
	//ダッシュ中はスタミナを消費する
	if (bCanSprint && bIsSprinting)
	{
		Stamina -= StaminaDrainRate * DeltaTime;

		//消費している間は、回復待ち時間をリセットし続ける
		RecoverDelayTimer = RecoverDelay;

		if (Stamina <= 0.0f)
		{
			Stamina = 0.0f;
			bCanSprint = false;
			bIsSprinting = false;
		}
		return;
	}

	//ダッシュをやめた直後は、回復を始めずに待つ
	if (RecoverDelayTimer > 0.0f)
	{
		RecoverDelayTimer -= DeltaTime;
		return;
	}

	//スタミナを回復する
	if (Stamina < MaxStamina)
	{
		Stamina = FMath::Min(Stamina + StaminaRecoverRate * DeltaTime, MaxStamina);
	}

	//枯渇していた場合は、閾値まで回復したらダッシュ可能に戻す
	if (!bCanSprint && Stamina >= MaxStamina * ResumeSprintRatio)
	{
		bCanSprint = true;
	}
}
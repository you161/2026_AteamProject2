
#include "Health/HealthManager.h"

AHealthManager::AHealthManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHealthManager::BeginPlay()
{
	Super::BeginPlay();
	
	//現在のHPを最大に初期化
	CurrentHealth = MaxHealth;
}

//ダメージ処理
void AHealthManager::TakeDamage()
{
	if (DamageAmount <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Max(0, CurrentHealth - DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("現在HP: %d / %d"), CurrentHealth, MaxHealth);
}

//HP参照関数
int32 AHealthManager::GetHealth()
{
	return CurrentHealth;
}
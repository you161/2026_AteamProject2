
#include "Health/HealthManager.h"
#include "Health/HitPointBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/GameJudge.h"
#include "Player/PlayerDamageData.h"
#include "Enemy/EnemyDamageData.h"
#include "Health/HealthData.h"
#include "Throwable/ThrowableManager.h"
#include "Engine/Engine.h"

AHealthManager::AHealthManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHealthManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < TotalDamage.Num(); i++)
	{
		TotalDamage[i] = 0;
	}

	bIsGameFinished = false;

	if (!IsValid(PlayerDamageData))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerDamageData is null in AHealthManager::TakeDamage"));
	}

	if (!IsValid(EnemyDamageData))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyDamageData is null in AHealthManager::TakeDamage"));
	}

	if (!IsValid(HealthData))
	{
		UE_LOG(LogTemp, Error, TEXT("HealthData is null in AHealthManager::TakeDamage"));
	}
	else
	{
		MaxHealth[CharactorType::Player] = HealthData->PlayerMaxHealth;
		MaxHealth[CharactorType::Enemy] = HealthData->EnemyMaxHealth;
	}

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogTemp, Error, TEXT("ThrowableManager is null in AHealthManager::TakeDamage"));
	}
}

//ダメージ処理
void AHealthManager::TakeDamage(CharactorType CurrentCharactorType)
{
	int32 Type = CurrentCharactorType;

	float CurrentDamage = 0.0f;

	if(!IsValid(PlayerDamageData)|| !IsValid(EnemyDamageData)|| !IsValid(ThrowableManager))
	{
		return;
	}

	if (Type == CharactorType::Player)
	{
		CurrentDamage = EnemyDamageData->EnemyAttackDamage;
	}
	else if(Type == CharactorType::Enemy)
	{
		const TArray<float>& Table = PlayerDamageData->PlayerAttackDamage;

		if (Table.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerAttackDamage is empty"));
			return;
		}

		//投げた時の個数を取得して、ダメージ量を決定する
		CollectedCount = ThrowableManager->GetThrownCount();
		const int32 Index = FMath::Clamp(CollectedCount, 0, Table.Num() - 1);
		CurrentDamage = Table[Index];
	}

	if (CurrentDamage <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentDamage is zero or negative in AHealthManager::TakeDamage"));
		return;
	}

	CurrentHealth[Type] = FMath::Max(0, CurrentHealth[Type] - CurrentDamage);
	TotalDamage[Type] += CurrentDamage;

	if (IsValid(HitPointBar[Type]))
	{
		HitPointBar[Type]->UpdateHitPointBar(MaxHealth[Type], TotalDamage[Type]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HitPointBar is null in AHealthManager::TakeDamage"));
	}
}

void AHealthManager::InitializeHealth(UHitPointBar* PlayerBar, UHitPointBar* EnemyBar)
{
	CurrentHealth.SetNum(MaxHealth.Num());
	HitPointBar.SetNum(MaxHealth.Num());

	for (int32 i = 0; i < MaxHealth.Num(); i++)
	{
		CurrentHealth[i] = MaxHealth[i];
	}

	HitPointBar[CharactorType::Player] = PlayerBar;
	HitPointBar[CharactorType::Enemy] = EnemyBar;

	if (IsValid(PlayerBar))
	{
		PlayerBar->OnHPBarEmptied.AddUObject(this, &AHealthManager::OnHPBarEmptied);
	}
	if (IsValid(EnemyBar))
	{
		EnemyBar->OnHPBarEmptied.AddUObject(this, &AHealthManager::OnHPBarEmptied);
	}
}

void AHealthManager::OnHPBarEmptied(UHitPointBar* EmptiedBar)
{
	if (bIsGameFinished)
	{
		return;  //二重遷移防止
	}

	bIsGameFinished = true;

	if (!IsValid(GameJudge))
	{
		UE_LOG(LogTemp, Error, TEXT("GameJudge is null in AHealthManager::OnHPBarEmptied"));
		return;
	}

	if (EmptiedBar == HitPointBar[Player])
	{
		GameJudge->GameOver();
	}
	else if (EmptiedBar == HitPointBar[Enemy])
	{
		GameJudge->GameClear();
	}
}

//HP参照関数
float AHealthManager::GetHealth(CharactorType CurrentCharactorType)const
{
	return CurrentHealth[static_cast<int32>(CurrentCharactorType)];
}
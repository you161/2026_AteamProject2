
#include "Health/HealthManager.h"
#include "Health/HitPointBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/GameJudge.h"
#include "Player/DamageAmountData.h"
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
}

//ダメージ処理
void AHealthManager::TakeDamage(CharactorType CurrentCharactorType)
{
	int32 Type = CurrentCharactorType;

	float CurrentDamage = 0.0f;

	if(!IsValid(DamageAmountData))
	{
		UE_LOG(LogTemp, Error, TEXT("DamageAmountData is null in AHealthManager::TakeDamage"));
		return;
	}

	if (Type == CharactorType::Player)
	{
		CurrentDamage = DamageAmountData->EnemyAttackDamage;
	}
	else if(Type == CharactorType::Enemy)
	{
		if (!IsValid(ThrowableManager))
		{
			UE_LOG(LogTemp, Error, TEXT("ThrowableManager is null in AHealthManager::TakeDamage"));
			return;
		}

		const TArray<float>& Table = DamageAmountData->PlayerAttackDamage;

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
		return;
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebugDamage && GEngine && Type == CharactorType::Enemy)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Red,
			FString::Printf(TEXT("個数: %d  ダメージ: %.1f"), CollectedCount, CurrentDamage));
	}
#endif

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
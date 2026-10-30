
#include "Health/HealthManager.h"
#include "HitPointBar.h"
#include "UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/GameJudge.h"

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

	if (DamageAmount[Type] <= 0)
	{
		return;
	}

	CurrentHealth[Type] = FMath::Max(0, CurrentHealth[Type] - DamageAmount[Type]);

	TotalDamage[Type] += DamageAmount[Type];

	if (IsValid(HitPointBar[Type]))
	{
		HitPointBar[Type]->UpdateHitPointBar(MaxHealth[Type], TotalDamage[Type]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HitPointBar is null in AHealthManager::TakeDamage"));
	}

	UE_LOG(LogTemp, Log, TEXT("Current HP: %d / %d"), CurrentHealth[Type], MaxHealth[Type]);
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
int32 AHealthManager::GetHealth(CharactorType CurrentCharactorType)const
{
	return CurrentHealth[static_cast<int32>(CurrentCharactorType)];
}
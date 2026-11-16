#include "Health/HealthManager.h"
#include "Health/HitPointBar.h"
#include "GameManager/GameJudge.h"
#include "Player/PlayerDamageData.h"
#include "Enemy/EnemyDamageData.h"
#include "Health/HealthData.h"
#include "Throwable/ThrowableManager.h"

DEFINE_LOG_CATEGORY(LogHealth);

namespace
{
	//キャラクターの種類数
	constexpr int32 CharacterCount = static_cast<int32>(ECharacterType::Count);
	//ECharacterType を配列の添字に変換する
	int32 ToIndex(ECharacterType Type)
	{
		return static_cast<int32>(Type);
	}
}

AHealthManager::AHealthManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHealthManager::BeginPlay()
{
	Super::BeginPlay();

	bIsGameFinished = false;
	EnsureInitialized();

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogHealth, Error, TEXT("ThrowableManager is null in AHealthManager::BeginPlay"));
		return;
	}

	ThrowableManager->OnHitEnemy.AddUObject(this, &AHealthManager::HandleThrowableHitEnemy);

	if (!IsValid(EnemyManager))
	{
		UE_LOG(LogHealth, Error, TEXT("ThrowableManager is null in EnemyManager::BeginPlay"));
		return;
	}

	EnemyManager->OnHitPlayer.AddUObject(this, &AHealthManager::HandleEnemyHitPlayer);
}

void AHealthManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(ThrowableManager))
	{
		ThrowableManager->OnHitEnemy.RemoveAll(this);
	}

	for (const TWeakObjectPtr<UHitPointBar>& Bar : HitPointBars)
	{
		if (Bar.IsValid())
		{
			Bar->OnHPBarEmptied.RemoveAll(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

//HPの配列を初期化する
void AHealthManager::EnsureInitialized()
{
	if (bIsInitialized)
	{
		return;
	}

	bIsInitialized = true;

	MaxHealth.Init(0.0f, CharacterCount);
	CurrentHealth.Init(0.0f, CharacterCount);
	HitPointBars.SetNum(CharacterCount);

	if (!IsValid(HealthData))
	{
		UE_LOG(LogHealth, Error, TEXT("HealthData is null in AHealthManager::EnsureInitialized"));
		return;
	}

	MaxHealth[ToIndex(ECharacterType::Player)] = HealthData->PlayerMaxHealth;
	MaxHealth[ToIndex(ECharacterType::Enemy)] = HealthData->EnemyMaxHealth;
	CurrentHealth = MaxHealth;
}

//指定したキャラクターにダメージを与える
void AHealthManager::ApplyDamage(ECharacterType Target, float DamageAmount)
{
	EnsureInitialized();

	const int32 Index = ToIndex(Target);

	if (!CurrentHealth.IsValidIndex(Index))
	{
		UE_LOG(LogHealth, Error, TEXT("Invalid target in AHealthManager::ApplyDamage"));
		return;
	}

	if (DamageAmount <= 0.0f)
	{
		UE_LOG(LogHealth, Warning, TEXT("DamageAmount is zero or negative in AHealthManager::ApplyDamage"));
		return;
	}

	CurrentHealth[Index] = FMath::Max(0.0f, CurrentHealth[Index] - DamageAmount);

	//ダメージを受けたことを通知する
	OnDamaged.Broadcast(Target);

	UHitPointBar* Bar = HitPointBars[Index].Get();

	if (!IsValid(Bar))
	{
		UE_LOG(LogHealth, Error, TEXT("HitPointBar is null in AHealthManager::ApplyDamage"));
		return;
	}

	Bar->UpdateHitPointBar(MaxHealth[Index], MaxHealth[Index] - CurrentHealth[Index]);
}

//敵の攻撃でプレイヤーにダメージを与える
void AHealthManager::HandleEnemyHitPlayer(EEnemyAttackType AttackType)
{
	if (!IsValid(EnemyDamageData))
	{
		UE_LOG(LogHealth, Error, TEXT("EnemyDamageData is null in AHealthManager::TakeDamage"));
		return;
	}

	if (AttackType == EEnemyAttackType::Normal)
	{
		ApplyDamage(ECharacterType::Player, EnemyDamageData->EnemyAttackDamage);
	}
}

//投擲オブジェクトが敵に当たった時の処理
void AHealthManager::HandleThrowableHitEnemy(int32 StackCount)
{
	if (!IsValid(PlayerDamageData))
	{
		UE_LOG(LogHealth, Error, TEXT("PlayerDamageData is null in AHealthManager::HandleThrowableHitEnemy"));
		return;
	}

	const TArray<float>& Table = PlayerDamageData->PlayerAttackDamage;

	if (Table.IsEmpty())
	{
		UE_LOG(LogHealth, Error, TEXT("PlayerAttackDamage is empty in AHealthManager::HandleThrowableHitEnemy"));
		return;
	}

	//投げた時の個数でダメージ量を決定する
	const int32 Index = FMath::Clamp(StackCount, 0, Table.Num() - 1);
	ApplyDamage(ECharacterType::Enemy, Table[Index]);
}

//HPバー設定
void AHealthManager::InitializeHealth(UHitPointBar* PlayerBar, UHitPointBar* EnemyBar)
{
	EnsureInitialized();

	BindHitPointBar(ECharacterType::Player, PlayerBar);
	BindHitPointBar(ECharacterType::Enemy, EnemyBar);
}

//HPバーを関連付けて空になった通知を受け取る
void AHealthManager::BindHitPointBar(ECharacterType Target, UHitPointBar* Bar)
{
	HitPointBars[ToIndex(Target)] = Bar;

	if (IsValid(Bar))
	{
		Bar->OnHPBarEmptied.AddUObject(this, &AHealthManager::OnHPBarEmptied);
	}
}

//HPバーが空になった時の処理
void AHealthManager::OnHPBarEmptied(UHitPointBar* EmptiedBar)
{
	//二重遷移防止
	if (bIsGameFinished)
	{
		return;
	}

	bIsGameFinished = true;

	if (!IsValid(GameJudge))
	{
		UE_LOG(LogHealth, Error, TEXT("GameJudge is null in AHealthManager::OnHPBarEmptied"));
		return;
	}

	if (EmptiedBar == HitPointBars[ToIndex(ECharacterType::Player)].Get())
	{
		GameJudge->GameOver();
	}
	else if (EmptiedBar == HitPointBars[ToIndex(ECharacterType::Enemy)].Get())
	{
		GameJudge->GameClear();
	}
}

//現在のHPを取得する
float AHealthManager::GetHealth(ECharacterType Target) const
{
	const int32 Index = ToIndex(Target);

	if (!CurrentHealth.IsValidIndex(Index))
	{
		return 0.0f;
	}

	return CurrentHealth[Index];
}
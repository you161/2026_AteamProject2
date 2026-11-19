#include "Player/PlayerPawn.h"
#include "Input/InputReceiver.h"
#include "Input/InputManager.h"
#include "Player/PlayerData.h"
#include "Components/CapsuleComponent.h"
#include "Throwable/ThrowableManager.h"
#include "Player/PlayerLevelManager.h"
#include "Player/StaminaManager.h"
#include "Health/HealthManager.h"
#include "Sound/SEManager.h"
#include "Component/DamageFlashComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerPawn);

namespace
{
	//カプセルの初期半径
	constexpr float DefaultCapsuleRadius = 34.0f;
	//カプセルの初期の半分の高さ
	constexpr float DefaultCapsuleHalfHeight = 88.0f;
	//ダッシュしていないときの速度倍率
	constexpr float NormalSpeedRate = 1.0f;
	//コリジョンプロファイル名
	const FName PlayerCollisionProfile(TEXT("Pawn"));
}

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	//CapsuleCollisionを生成してルートにする
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	SetRootComponent(CapsuleCollision);
	CapsuleCollision->InitCapsuleSize(DefaultCapsuleRadius, DefaultCapsuleHalfHeight);
	CapsuleCollision->SetCollisionProfileName(PlayerCollisionProfile);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//BeginPlay時点ですでにPossessされている場合に備える
	InitializeInputManager();

	CurrentDirection = FVector::ZeroVector;

	DamageFlashComponent = FindComponentByClass<UDamageFlashComponent>();

	if (IsValid(HealthManager))
	{
		//ダメージ通知を受け取るよう登録する
		HealthManager->OnDamaged.AddUObject(this, &APlayerPawn::OnDamaged);
	}

	ValidateReferences();

	if (IsValid(ThrowableManager))
	{
		ThrowableManager->OnThrowStarted.AddUObject(this, &APlayerPawn::HandleThrowStarted);
	}

	bIsMoving = false;
}

void APlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//登録を解除する
	if (IsValid(HealthManager))
	{
		HealthManager->OnDamaged.RemoveAll(this);
	}

	if (IsValid(ThrowableManager))
	{
		ThrowableManager->OnThrowStarted.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Possess前や必要な設定がない間は何もしない
	if (!HasRequiredReferences())
	{
		return;
	}

	Move(DeltaTime);
	Rotate(DeltaTime);
}

//必要な参照が設定されているか確認してログに出す
void APlayerPawn::ValidateReferences() const
{
	if (!IsValid(PlayerData))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("PlayerData is not set in PlayerPawn."));
	}
	else
	{
		//速度テーブルは毎フレーム確認するとログが溢れるので、ここで一度だけ確認する
		if (PlayerData->MoveSpeedTable.IsEmpty())
		{
			UE_LOG(LogPlayerPawn, Error, TEXT("PlayerData->MoveSpeedTable is empty."));
		}

		//移動速度が設定されているか確認(レベルが1スタートのため1からループスタート)
		for (int32 Index = 1; Index < PlayerData->MoveSpeedTable.Num(); ++Index)
		{
			if (PlayerData->MoveSpeedTable[Index].Values.IsEmpty())
			{
				UE_LOG(LogPlayerPawn, Error, TEXT("MoveSpeedTable[%d] is empty."), Index);
			}
		}
	}

	if (!IsValid(Camera))
	{
		UE_LOG(LogPlayerPawn, Error, TEXT("Camera is not set in PlayerPawn."));
	}

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("ThrowableManager is not set in PlayerPawn."));
	}

	if (!IsValid(PlayerLevelManager))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("PlayerLevelManager is not set in PlayerPawn."));
	}

	if (!IsValid(StaminaManager))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("StaminaManager is not set in PlayerPawn."));
	}

	if (!IsValid(HealthManager))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("HealthManager is not set in PlayerPawn."));
	}

	if (!IsValid(SEManager))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("SEManager is not set in PlayerPawn."));
	}
}

//Tickで必要な参照がそろっているかを返す
bool APlayerPawn::HasRequiredReferences() const
{
	return IsValid(InputManager)
		&& IsValid(PlayerData)
		&& IsValid(Camera)
		&& IsValid(ThrowableManager)
		&& IsValid(PlayerLevelManager)
		&& IsValid(StaminaManager)
		&& IsValid(SEManager);
}

//移動処理
void APlayerPawn::Move(float DeltaTime)
{
	UpdateMoveDirection(InputManager->GetMoveInput());

	//ダッシュ中は速度倍率を適用する
	const bool bIsDashing = InputManager->GetIsDashing()
		&& !CurrentDirection.IsNearlyZero()
		&& StaminaManager->GetCanSprint();

	StaminaManager->SetIsSprinting(bIsDashing);

	const float SpeedRate = bIsDashing ? PlayerData->DashSpeedRate : NormalSpeedRate;
	const FVector Delta = CurrentDirection * GetMoveSpeed() * SpeedRate * DeltaTime;

	bIsMoving = !Delta.IsNearlyZero();

	//移動状態を更新する
	if (bIsMoving)
	{
		MoveWithSlide(Delta);
		MoveState = bIsDashing ? EPlayerMoveState::Dash : EPlayerMoveState::Walk;
		bIsDashing ? SEManager->PlaySE(ESEType::PlayerDash) : SEManager->PlaySE(ESEType::PlayerWalk);
		bIsDashing ? SEManager->StopSE(ESEType::PlayerWalk) : SEManager->StopSE(ESEType::PlayerDash);
	}
	else
	{
		MoveState = EPlayerMoveState::Idle;
		SEManager->StopSE(ESEType::PlayerDash);
		SEManager->StopSE(ESEType::PlayerWalk);
	}
}

//入力とカメラの向きから移動方向を更新する
void APlayerPawn::UpdateMoveDirection(const FVector2D& MoveInput)
{
	//カメラの水平方向だけを使う
	const FVector CameraForward = Camera->GetActorForwardVector().GetSafeNormal2D();
	const FVector CameraRight = Camera->GetActorRightVector().GetSafeNormal2D();

	//入力をカメラの向き基準のワールド方向に変換
	CurrentDirection = (CameraForward * MoveInput.Y + CameraRight * MoveInput.X).GetSafeNormal();
}

//現在のレベルと所持数に応じた基本移動速度を返す
float APlayerPawn::GetMoveSpeed() const
{
	const TArray<FMoveSpeed>& Table = PlayerData->MoveSpeedTable;

	if (Table.IsEmpty())
	{
		return 0.0f;
	}

	//現在のレベルに対応する行を取得する
	const int32 LevelIndex = FMath::Clamp(PlayerLevelManager->GetLevel(), 0, Table.Num() - 1);
	const TArray<float>& Row = Table[LevelIndex].Values;

	if (Row.IsEmpty())
	{
		return 0.0f;
	}

	//集めた個数で列を決める
	const int32 CountIndex = FMath::Clamp(ThrowableManager->GetCollectedCount(), 0, Row.Num() - 1);
	return Row[CountIndex];
}

//衝突したら壁に沿って滑るように移動する
void APlayerPawn::MoveWithSlide(const FVector& Delta)
{
	FHitResult Hit;
	AddActorWorldOffset(Delta, true, &Hit);

	if (!Hit.IsValidBlockingHit())
	{
		return;
	}

	//壁に沿った方向へ残りの移動量だけ動く
	FVector Slide = FVector::VectorPlaneProject(Delta, Hit.Normal) * (1.0f - Hit.Time);
	Slide.Z = 0.0f;
	AddActorWorldOffset(Slide, true);
}

//回転処理
void APlayerPawn::Rotate(float DeltaTime)
{
	//移動入力がない場合は回転しない
	if (CurrentDirection.IsNearlyZero())
	{
		return;
	}

	//一定の速度で移動方向へ回転する
	const FRotator NewRotation = FMath::RInterpConstantTo(
		GetActorRotation(),
		CurrentDirection.Rotation(),
		DeltaTime,
		PlayerData->RotateSpeed
	);

	SetActorRotation(NewRotation);
}

//コントローラーがポーンを制御する際に呼ばれる
void APlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeInputManager();
}

//コントローラーがポーンの制御を手放す際に呼ばれる
void APlayerPawn::UnPossessed()
{
	Super::UnPossessed();

	//古いコントローラーのInputManagerを参照し続けないようにする
	InputManager = nullptr;
	CurrentDirection = FVector::ZeroVector;
}

//InputManagerを初期化する関数
void APlayerPawn::InitializeInputManager()
{
	AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetController());

	if (!IsValid(InputReceiver))
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("InputReceiver is invalid."));
		return;
	}

	InputManager = InputReceiver->GetInputManager();
}

//ダメージを受けた時の処理
void APlayerPawn::OnDamaged(ECharacterType DamagedType, int32 DamageLevel)
{
	//自分宛てでなければ何もしない
	if (DamagedType != ECharacterType::Player)
	{
		return;
	}

	if (IsValid(DamageFlashComponent))
	{
		DamageFlashComponent->PlayFlash();
	}
}
//オブジェクトを投げた通知を受け取りアニメーションクラスに通知を飛ばす
void APlayerPawn::HandleThrowStarted()
{
	OnThrow.Broadcast();

	if (IsValid(SEManager))
	{
		SEManager->PlaySE(ESEType::PlayerAttack);
	}
}

//移動中かどうかを取得する関数
bool APlayerPawn::GetIsMoving()const
{
	return bIsMoving;
}

//プレイヤーがオブジェクトを所持しているかを取得する
bool APlayerPawn::GetHasThrowable() const
{
	return IsValid(ThrowableManager) && ThrowableManager->GetCollectedCount() > 0;
}
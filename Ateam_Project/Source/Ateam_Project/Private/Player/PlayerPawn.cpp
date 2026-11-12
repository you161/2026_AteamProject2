#include "Player/PlayerPawn.h"
#include "Input/InputReceiver.h"
#include "Input/InputManager.h"
#include "Player/PlayerData.h"
#include "Components/CapsuleComponent.h"
#include "Throwable/ThrowableManager.h"
#include "Player/PlayerLevelManager.h"
#include "Player/StaminaManager.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	//CapsuleCollisionを生成してルートにする
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	SetRootComponent(CapsuleCollision);
	CapsuleCollision->InitCapsuleSize(34.0f, 88.0f);
	CapsuleCollision->SetCollisionProfileName(TEXT("Pawn"));
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//BeginPlay時点ですでにPossessされている場合に備える
	InitializeInputManager();

	CurrentDirection = FVector::ZeroVector;

	if (!IsValid(InputManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("InputManager is not set in PlayerPawn."));
		return;
	}

	if (IsValid(Camera))
	{
		CurrentCameraForward = Camera->GetActorForwardVector();
		CurrentCameraRight = Camera->GetActorRightVector();
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Camera is not set in PlayerPawn."));
	}

	if (!IsValid(PlayerData))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerData is not set in PlayerPawn."));
		return;
	}

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ThrowableManager is not set in PlayerPawn."));
		return;
	}

	if (!IsValid(PlayerLevelManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerLevelManager is not set in PlayerPawn."));
		return;
	}

	if (!IsValid(StaminaManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("StaminaManager is not set in PlayerPawn."));
		return;
	}

	bisMoving = false;
	bhasObject = false;
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Possess前や必要な設定がない間は何もしない
	if (!IsValid(InputManager)	   ||
		!IsValid(PlayerData)	   ||
		!IsValid(Camera)		   ||
		!IsValid(ThrowableManager) || 
		!IsValid(PlayerLevelManager) ||
		!IsValid(StaminaManager))
	{
		return;
	}

	Move(DeltaTime);
	Rotate(DeltaTime);
}

//移動処理
void APlayerPawn::Move(float DeltaTime)
{
	const FVector2D MoveInput = InputManager->GetMoveInput();

	if (!IsValid(Camera))
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera is not set in PlayerPawn."));
		return;
	}

	CurrentCameraForward = Camera->GetActorForwardVector();
	CurrentCameraRight = Camera->GetActorRightVector();

	CurrentCameraForward.Z = 0.0f;
	CurrentCameraRight.Z = 0.0f;

	CurrentCameraForward.Normalize();
	CurrentCameraRight.Normalize();

	//入力をカメラの向き基準のワールド方向に変換
	CurrentDirection = (CurrentCameraForward * MoveInput.Y + CurrentCameraRight * MoveInput.X).GetSafeNormal();
	//移動速度のデータを取得する
	const TArray<FMoveSpeed>& Table = PlayerData->MoveSpeedTable;

	if (Table.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerData->MoveSpeedTable is empty"));
		return;
	}

	//現在のレベルを取得する
	const int CurrentLevel = FMath::Clamp(PlayerLevelManager->GetLevel(), 0, Table.Num() - 1);
	//現在のレベルに対応する移動速度の行を取得する
	const TArray<float>& Row = Table[CurrentLevel].Values;

	if (Row.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MoveSpeedTable[%d] is empty"), CurrentLevel);
		return;
	}

	//集めた個数を取得して、移動速度を決定する
	const int32 Index = FMath::Clamp(ThrowableManager->GetCollectedCount(), 0, Row.Num() - 1);
	//ダッシュ中は速度倍率を適用する
	const bool bIsDashing = InputManager->GetIsDashing() &&
							!CurrentDirection.IsNearlyZero() && 
							StaminaManager->GetCanSprint();

	StaminaManager->SetIsSprinting(bIsDashing);

	const float DashRate = bIsDashing ? PlayerData->DashSpeedRate : 1.0f;
	const FVector Delta = CurrentDirection * Row[Index] * DashRate * DeltaTime;

	if (!Delta.IsNearlyZero())
	{
		FHitResult Hit;
		AddActorWorldOffset(Delta, true, &Hit);

		//衝突が発生した場合、スライド移動を行う
		if (Hit.IsValidBlockingHit())
		{
			//スライド移動の計算
			FVector Slide = FVector::VectorPlaneProject(Delta, Hit.Normal) * (1.0f - Hit.Time);
			Slide.Z = 0.0f;
			AddActorWorldOffset(Slide, true);
		}

		bisMoving = true;
	}
	else
	{
		bisMoving = false;
	}
}

//回転処理
void APlayerPawn::Rotate(float DeltaTime)
{
	//移動入力がない場合は回転しない
	if (CurrentDirection.IsNearlyZero())
	{
		return;
	}

	//水平方向だけを回転に使用する
	FVector Direction(CurrentDirection.X, CurrentDirection.Y, 0.0f);

	if (Direction.IsNearlyZero())
	{
		return;
	}

	Direction.Normalize();

	//移動方向から目標回転を求める
	const FRotator TargetRotation = Direction.Rotation();

	//現在の回転を取得する
	const FRotator CurrentRotation = GetActorRotation();

	//一定の角速度で目標方向へ回転する
	const FRotator NewRotation = FMath::RInterpConstantTo(
		CurrentRotation,
		TargetRotation,
		DeltaTime,
		PlayerData->RotateSpeed
	);

	SetActorRotation(NewRotation);
}

//PossessedByは、コントローラーがポーンを制御する際に呼び出される関数
void APlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeInputManager();
}

//UnPossessedは、コントローラーがポーンの制御を手放す際に呼び出される関数
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
	AController* CurrentController = GetController();

	AInputReceiver* InputReceiver = Cast<AInputReceiver>(CurrentController);

	if (!IsValid(InputReceiver))
	{
		UE_LOG(LogTemp,Warning,TEXT("[PlayerPawn] InputReceiver is invalid."));
		return;
	}

	InputManager = InputReceiver->GetInputManager();
}
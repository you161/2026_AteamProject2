#include "Player/PlayerPawn.h"
#include "Input/InputReceiver.h"
#include "Input/InputManager.h"
#include "Player/PlayerData.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(PlayerData))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerData is not set in PlayerPawn."));
	}

	if (!IsValid(Camera))
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera is not set in PlayerPawn."));
	}

	//BeginPlay時点ですでにPossessされている場合に備える
	InitializeInputManager();

	CurrentDirection = FVector::ZeroVector;

	if (IsValid(Camera))
	{
		CurrentCameraForward = Camera->GetActorForwardVector();
		CurrentCameraRight = Camera->GetActorRightVector();
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Camera is not set in PlayerPawn.")
		);
	}

	isMoving = false;
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Possess前や必要な設定がない間は何もしない
	if (!IsValid(InputManager) || !IsValid(PlayerData) || !IsValid(Camera))
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

	if (!isMoving)
	{
		//移動中じゃないときはカメラの向きを更新する
		CurrentCameraForward = Camera->GetActorForwardVector();
		CurrentCameraRight = Camera->GetActorRightVector();

		CurrentCameraForward.Z = 0.0f;
		CurrentCameraRight.Z = 0.0f;

		CurrentCameraForward.Normalize();
		CurrentCameraRight.Normalize();
	}

	//入力をカメラの向き基準のワールド方向に変換
	CurrentDirection = (CurrentCameraForward * MoveInput.Y + CurrentCameraRight * MoveInput.X).GetSafeNormal();

	const FVector Delta = CurrentDirection * PlayerData->MoveSpeed * DeltaTime;

	if (!Delta.IsNearlyZero())
	{
		AddActorWorldOffset(Delta, true);
		isMoving = true;
	}
	else
	{
		isMoving = false;
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

void APlayerPawn::Jump(float DeltaTime)
{
	bIsGrounded = IsGrounded();

	if (bIsGrounded)
	{
		if (VerticalVelocity < 0.0f)
		{
			VerticalVelocity = 0.0f;
		}

		if (InputManager->IsJumpPressed())
		{
			VerticalVelocity = PlayerData->JumpSpeed;
		}
	}
	else
	{
		VerticalVelocity -= PlayerData->Gravity * DeltaTime;
	}

	FHitResult Hit;
	AddActorWorldOffset(FVector(0, 0, VerticalVelocity * DeltaTime), true, &Hit);

	// 天井や床にぶつかったら速度をリセット
	if (Hit.IsValidBlockingHit())
	{
		VerticalVelocity = 0.0f;
	}
}

bool APlayerPawn::IsGrounded() const
{
	// TODO: 地面判定処理を実装する
	return false;
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
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[PlayerPawn] InputReceiver is invalid.")
		);
		return;
	}

	InputManager = InputReceiver->GetInputManager();
}
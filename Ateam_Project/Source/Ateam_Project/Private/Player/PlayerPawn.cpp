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

	if (PlayerData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerData is not set in PlayerPawn."));
	}

	if (Camera == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera is not set in PlayerPawn."));
	}

	//BeginPlay時点ですでにPossessされている場合に備える
	InitializeInputManager();
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Possess前や必要な設定がない間は何もしない
	if (InputManager == nullptr || PlayerData == nullptr || Camera == nullptr)
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

	FVector Forward = Camera->GetActorForwardVector();
	FVector Right = Camera->GetActorRightVector();
	Forward.Z = 0.0f;
	Right.Z = 0.0f;
	Forward.Normalize();
	Right.Normalize();

	//入力をカメラの向き基準のワールド方向に変換
	CurrentDirection = (Forward * MoveInput.Y + Right * MoveInput.X).GetSafeNormal();

	const FVector Delta = CurrentDirection * PlayerData->MoveSpeed * DeltaTime;

	if (!Delta.IsNearlyZero())
	{
		AddActorWorldOffset(Delta, true);
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
	if (AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetController()))
	{
		InputManager = InputReceiver->GetInputManager();
	}
}
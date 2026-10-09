
#include "Player/PlayerPawn.h"
#include "Input/InputReceiver.h"
#include "Input/InputManager.h"
#include "Player/PlayerData.h"
#include "GameFramework/Actor.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

    //BeginPlay時点ですでにPossessされている場合に備える
    InitializeInputManager();
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (InputManager == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("InputManager is null in PlayerPawn."));
		return;
    }

	if (PlayerData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerData is not set in PlayerPawn."));
		return;
	}

    Move(DeltaTime);
    Rotate(DeltaTime);
}

//移動処理
void APlayerPawn::Move(float DeltaTime)
{
    const FVector2D MoveInput = InputManager->GetMoveInput();

	if (Camera == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera is not set in PlayerPawn."));
		return;
	}

	FVector Forward = Camera->GetActorForwardVector();
	FVector Right = Camera->GetActorRightVector();
    Forward.Z = 0.0f;
    Right.Z = 0.0f;
    Forward.Normalize();
    Right.Normalize();

    //入力をアクターの向き基準のワールド方向に変換
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
    FVector Direction = FVector3d(CurrentDirection.X, CurrentDirection.Y, 0.0f);

    if (Direction.IsNearlyZero())
    {
        return;
    }

    Direction.Normalize();

    //移動方向から目標回転を求める
    const FRotator TargetRotation = Direction.Rotation();

    //現在の回転を取得する
    const FRotator CurrentRotation = GetActorRotation();

    //現在のYawと目標Yawの角度差を求める
    const float Angle = FMath::Abs(
        FMath::FindDeltaAngleDegrees(
            CurrentRotation.Yaw,
            TargetRotation.Yaw
        )
    );

    //大きく方向転換する場合は回転速度を倍にする
    float CurrentRotationSpeed = PlayerData->RotateSpeed;

    //if (Angle >= 150.0f)
    //{
    //    CurrentRotationSpeed *= 2.0f;
    //}

    //一定の角速度で目標方向へ回転する
    const FRotator NewRotation = FMath::RInterpConstantTo(
        CurrentRotation,
        TargetRotation,
        DeltaTime,
        CurrentRotationSpeed
    );

    SetActorRotation(NewRotation);
}

//PossessedByは、コントローラーがポーンを制御する際に呼び出される関数
void APlayerPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (NewController == nullptr)
    {
        UE_LOG(LogTemp, Warning,TEXT("PossessedBy: NewController is null."));
        return;
    }

    UE_LOG(LogTemp, Warning,TEXT("PossessedBy: Controller Class = %s"),*NewController->GetClass()->GetName());

    InitializeInputManager();
}

//InputManagerを初期化する関数
void APlayerPawn::InitializeInputManager()
{
    AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetController());

    if (InputReceiver == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputReceiver is null in InitializeInputManager."));
        return;
    }

    InputManager = InputReceiver->GetInputManager();

    if (InputManager == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputManager is null in InitializeInputManager."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("InputManager initialized successfully."));
}
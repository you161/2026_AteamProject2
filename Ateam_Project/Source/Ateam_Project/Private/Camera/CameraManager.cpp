
#include "Camera/CameraManager.h"
#include "Input/InputManager.h"
#include "Input/InputReceiver.h"
#include "Player/PlayerData.h"

ACameraManager::ACameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraManager::BeginPlay()
{
	Super::BeginPlay();

	//InputManagerを初期化
    InitializeInputManager();

    if (!IsValid(CameraActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("CameraActor is not set."));
        return;
    }

    if (!IsValid(PlayerData))
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerData is not set."));
        return;
    }

	//CameraActorの初期回転をYawとPitchに設定
	Yaw = CameraActor->GetActorRotation().Yaw;
    Pitch = CameraActor->GetActorRotation().Pitch;
}

void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RotateCamera();
}

//カメラの回転を処理する関数
void ACameraManager::RotateCamera()
{
    //必要なオブジェクトが有効でない場合は処理を中断
    if (!IsValid(InputManager) || !IsValid(PlayerData)
        || !IsValid(CameraActor) || !IsValid(TargetActor))
    {
        return;
    }

	//InputManagerからマウスの入力を取得
	const FVector2D& LookInput = InputManager->GetMouseInput();

	//マウスの入力がある場合、YawとPitchを更新
	if (!LookInput.IsZero())
	{
		Yaw += LookInput.X * PlayerData->CameraSensitivity;
		Pitch += LookInput.Y * PlayerData->CameraSensitivity;
	}

    //Yawの範囲を制限
    Yaw = FMath::Clamp(Yaw, PlayerData->MinYaw, PlayerData->MaxYaw);
	//Pitchの範囲を制限
    Pitch = FMath::Clamp(Pitch, PlayerData->MinPitch, PlayerData->MaxPitch);

    //プレイヤーの注視位置
    FVector TargetLocation = TargetActor->GetActorLocation();

    //カメラの角度
    const FRotator CameraRotation(Pitch, Yaw, 0.0f);

    //カメラからプレイヤーへ向かう方向とは逆向きに配置
    const FVector CameraDirection = CameraRotation.Vector();

	//カメラの位置を計算
	const FVector CameraLocation = TargetLocation - CameraDirection * PlayerData->CameraDistance
                                 + FVector(0.0f, 0.0f, PlayerData->CameraHeightOffset);

	//カメラの位置と回転を設定
    CameraActor->SetActorLocation(CameraLocation);
    CameraActor->SetActorRotation(CameraRotation);
}

//InputManagerを初期化する関数
void ACameraManager::InitializeInputManager()
{
    if (AInputReceiver* InputReceiver = Cast<AInputReceiver>(GetWorld()->GetFirstPlayerController()))
    {
        InputManager = InputReceiver->GetInputManager();
    }
}
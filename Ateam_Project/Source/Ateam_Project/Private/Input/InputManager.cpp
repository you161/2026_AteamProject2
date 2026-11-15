#include "Input/InputManager.h"

UInputManager::UInputManager()
{
}

//移動入力を設定する
void UInputManager::SetMoveInput(const FVector2D& Input)
{
	MoveInput = Input;
}

//移動入力を取得する
FVector2D UInputManager::GetMoveInput() const
{
	return MoveInput;
}

//マウスの入力値を設定する
void UInputManager::SetMouseInput(const FVector2D& Input)
{
	MouseInput = Input;
}

//マウスの入力値を取得する
FVector2D UInputManager::GetMouseInput() const
{
	return MouseInput;
}

//コントローラーの視点入力を設定する
void UInputManager::SetGamepadLookInput(const FVector2D& Input)
{
	GamepadLookInput = Input;
}

//コントローラーの視点入力を取得する
FVector2D UInputManager::GetGamepadLookInput() const
{
	return GamepadLookInput;
}

//ダッシュ中かどうかを設定する
void UInputManager::SetIsDashing(bool bDashing)
{
	bIsDashing = bDashing;
}

//ダッシュ中かどうかを取得する
bool UInputManager::GetIsDashing() const
{
	return bIsDashing;
}

//インタラクト入力があったことを通知する
void UInputManager::NotifyInteract()
{
	OnInteract.Broadcast();
}
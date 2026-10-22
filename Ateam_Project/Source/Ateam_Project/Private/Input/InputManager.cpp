
#include "Input/InputManager.h"

UInputManager::UInputManager(){}

//入力値を設定する関数
void UInputManager::SetMoveInput(const FVector2D& Input)
{
    MoveInput = Input;
}

//入力値を取得する関数
FVector2D UInputManager::GetMoveInput() const
{
	return MoveInput;
}

//マウスの入力値を設定する関数
void UInputManager::SetMouseInput(const FVector2D& Input)
{
	MouseInput = Input;
}

//マウスの入力値を取得する関数
FVector2D UInputManager::GetMouseInput() const
{
	return MouseInput;
}

//インタラクション中かどうかを設定する関数
void UInputManager::SetIsInteracting(bool bInteracting)
{
	isInteracting = bInteracting;
}

//インタラクション中かどうかを取得する関数
bool UInputManager::GetIsInteracting() const
{
	return isInteracting;
}
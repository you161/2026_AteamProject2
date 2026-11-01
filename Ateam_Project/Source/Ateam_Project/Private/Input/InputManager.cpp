
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
	bisInteracting = bInteracting;
}

//インタラクション中かどうかを取得する関数
bool UInputManager::ConsumeInteract()
{
	const bool bResult = bisInteracting;
	bisInteracting = false;
	return bResult;
}

//ダッシュ中かどうかを設定する関数
void UInputManager::SetIsDashing(bool bDashing)
{
	bisDashing = bDashing;
}

//ダッシュ中かどうかを取得する関数
bool UInputManager::GetIsDashing() const
{
	return bisDashing;
}

#include "Input/InputManager.h"

UInputManager::UInputManager(){}

void UInputManager::SetMoveInput(const FVector2D& Input)
{
    MoveInput = Input;
}

void UInputManager::SetJumpPressed(bool bPressed)
{
	bJumpPressed = bPressed;
}

void UInputManager::SetMouseInput(const FVector2D& Input)
{
	MouseInput = Input;
}

FVector2D UInputManager::GetMoveInput() const
{
	return MoveInput;
}

bool UInputManager::IsJumpPressed() const
{
	return bJumpPressed;
}

FVector2D UInputManager::GetMouseInput() const
{
	return MouseInput;
}
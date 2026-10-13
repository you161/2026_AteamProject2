
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

FVector2D UInputManager::GetMoveInput() const
{
	return MoveInput;
}

bool UInputManager::IsJumpPressed() const
{
	return bJumpPressed;
}
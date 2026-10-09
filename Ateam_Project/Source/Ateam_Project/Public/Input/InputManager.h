
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputManager.generated.h"

/**
 * 入力管理クラス
 */
UCLASS()
class ATEAM_PROJECT_API UInputManager : public UObject
{
	GENERATED_BODY()
public:
	UInputManager();

	void SetMoveInput(const FVector2D& Input);
	FVector2D GetMoveInput() const;

	void SetJumpPressed(bool bPressed);
	bool IsJumpPressed() const;

private:
	FVector2D MoveInput = FVector2D::ZeroVector;
	bool bJumpPressed = false;
};
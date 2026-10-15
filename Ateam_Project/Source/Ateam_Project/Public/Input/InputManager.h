
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

	//入力値を設定する関数
	void SetMoveInput(const FVector2D& Input);
	//マウスの入力値を設定する関数
	void SetMouseInput(const FVector2D& Input);
	//ジャンプ入力の状態を設定する関数
	void SetJumpPressed(bool bPressed);

	//入力値を取得する関数
	FVector2D GetMoveInput() const;
	//マウスの入力値を取得する関数
	FVector2D GetMouseInput() const;
	//ジャンプ入力の状態を取得する関数
	bool IsJumpPressed() const;

private:

	//入力値を保持する変数
	FVector2D MoveInput = FVector2D::ZeroVector;
	//マウスの入力値を保持する変数
	FVector2D MouseInput = FVector2D::ZeroVector;
	//ジャンプ入力の状態を保持する変数
	bool bJumpPressed = false;
};
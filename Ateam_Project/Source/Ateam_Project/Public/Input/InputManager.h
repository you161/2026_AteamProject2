#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputManager.generated.h"

//インタラクト入力が行われた時に通知するデリゲート
DECLARE_MULTICAST_DELEGATE(FOnInteractInput);

/**
 * 入力管理クラス
 */
UCLASS()
class ATEAM_PROJECT_API UInputManager : public UObject
{
	GENERATED_BODY()

public:

	UInputManager();

	//移動入力を設定する
	void SetMoveInput(const FVector2D& Input);
	//マウスの入力値を設定する
	void SetMouseInput(const FVector2D& Input);
	//コントローラーの視点入力を設定する
	void SetGamepadLookInput(const FVector2D& Input);
	//ダッシュ中かどうかを設定する
	void SetIsDashing(bool bDashing);
	//インタラクト入力があったことを通知する
	void NotifyInteract();

	//移動入力を取得する
	FVector2D GetMoveInput() const;
	//マウスの入力値を取得する
	FVector2D GetMouseInput() const;
	//コントローラーの視点入力を取得する
	FVector2D GetGamepadLookInput() const;
	//ダッシュ中かどうかを取得する
	bool GetIsDashing() const;

	//インタラクト入力があった時に呼ばれるデリゲート
	FOnInteractInput OnInteract;

private:
	//移動入力値
	FVector2D MoveInput = FVector2D::ZeroVector;
	//マウスの入力値
	FVector2D MouseInput = FVector2D::ZeroVector;
	//コントローラーの視点入力値
	FVector2D GamepadLookInput = FVector2D::ZeroVector;
	//ダッシュ中かどうか
	bool bIsDashing = false;
};
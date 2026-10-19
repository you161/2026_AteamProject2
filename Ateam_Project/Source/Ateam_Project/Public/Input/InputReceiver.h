#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputReceiver.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputManager;

/**
 * 入力を受け取るプレイヤーコントローラー
 */
UCLASS()
class ATEAM_PROJECT_API AInputReceiver : public APlayerController
{
	GENERATED_BODY()

public:
	AInputReceiver();

	//InputManagerを取得する
	UInputManager* GetInputManager();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	//入力マッピングコンテキスト
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	//移動入力
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction = nullptr;

	//ジャンプ入力
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction = nullptr;

	//マウス入力
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction = nullptr;

	//入力管理を行うオブジェクト
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Input")
	TObjectPtr<UInputManager> InputManager = nullptr;

	void Move(const FInputActionValue& Value);
	void Jump();
	void JumpReleased();
	void Look(const FInputActionValue& Value);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaminaManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, Ratio, bool, bCanSprint);

UCLASS()
class ATEAM_PROJECT_API AStaminaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaminaManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaChanged OnStaminaChanged;

	//スタミナの減少と回復を制御する関数
	void SetIsSprinting(bool bSprinting) { bIsSprinting = bSprinting; }
	//ダッシュ中かどうかを返す
	bool GetIsSprinting() const { return bIsSprinting; }
	//ダッシュ可能かどうかを返す
	bool GetCanSprint() const { return bCanSprint; }
	//現在のスタミナ割合
	float GetStaminaRatio() const { return (MaxStamina > 0.0f) ? Stamina / MaxStamina : 0.0f; }

private:
	//スタミナの減少と回復を処理する関数
	void Sprint(float DeltaTime);

protected:
	//最大スタミナ
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (ClampMin = "1.0"))
	float MaxStamina = 100.0f;
	//ダッシュ中の1秒あたりの消費量
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (ClampMin = "0.0"))
	float StaminaDrainRate = 20.0f;
	//1秒あたりの回復量
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (ClampMin = "0.0"))
	float StaminaRecoverRate = 15.0f;
	//ダッシュをやめてから回復が始まるまでの待ち時間
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (ClampMin = "0.0"))
	float RecoverDelay = 1.0f;
	//枯渇後、再びダッシュできるようになる回復割合
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ResumeSprintRatio = 0.2f;
	//デバッグ表示のON/OFF
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebug = true;

private:
	//ダッシュ可能かどうかを保持する変数
	bool bCanSprint = true;
	//ダッシュ中かどうかを保持する変数
	bool bIsSprinting = false;
	//現在のスタミナ値
	float Stamina = 0.0f;
	//回復待ちの残り時間
	float RecoverDelayTimer = 0.0f;
	//ログの間引き用タイマー
	float DebugLogTimer = 0.0f;
	//前回通知した値(変化したときだけ通知するため)
	float LastNotifiedStamina = -1.0f;
	bool bLastNotifiedCanSprint = true;
};
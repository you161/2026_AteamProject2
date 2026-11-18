#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/PlayerMoveState.h"
#include "PlayerAnimation.generated.h"

class APlayerPawn;
class UAnimMontage;

/**
 * プレイヤーのアニメーションに必要な値を保持するクラス
 */
UCLASS()
class ATEAM_PROJECT_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//投げるアニメーションを再生する関数
	void PlayThrowMontage();

protected:
	virtual void NativeUninitializeAnimation() override;

private:
	//投げた通知を受けてMontageを再生する
	void HandleThrow();

protected:
	//プレイヤーがオブジェクトを所持しているかどうか
	UPROPERTY(BlueprintReadOnly, Category = "PlayerAnimation")
	bool bHasThrowable = false;
	//移動状態
	UPROPERTY(BlueprintReadOnly, Category = "PlayerAnimation")
	EPlayerMoveState PlayerMoveState = EPlayerMoveState::Idle;
	//投げるときのアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnimation")
	TObjectPtr<UAnimMontage> ThrowMontage = nullptr;

private:
	//PlayerPawn
	UPROPERTY(Transient)
	TObjectPtr<APlayerPawn> PlayerPawn = nullptr;
	//アニメーションを再生するメッシュ
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> MeshComponent = nullptr;
};
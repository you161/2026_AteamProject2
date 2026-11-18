#include "Player/PlayerAnimation.h"
#include "Player/PlayerPawn.h"

void UPlayerAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//初期化時に1回だけCastしてキャッシュする
	PlayerPawn = Cast<APlayerPawn>(TryGetPawnOwner());

	if (IsValid(PlayerPawn))
	{
		PlayerPawn->OnThrow.AddUObject(this, &UPlayerAnimation::HandleThrow);
	}

	PlayerMoveState = EPlayerMoveState::Idle;
}

void UPlayerAnimation::NativeUninitializeAnimation()
{
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->OnThrow.RemoveAll(this);
	}

	Super::NativeUninitializeAnimation();
}

void UPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//初期化時点で取得できなかった場合に備えて再取得する
	if (!IsValid(PlayerPawn))
	{
		PlayerPawn = Cast<APlayerPawn>(TryGetPawnOwner());
	}

	//エディタのプレビュー中などはPawnがいないので何もしない
	if (!IsValid(PlayerPawn))
	{
		PlayerMoveState = EPlayerMoveState::Idle;
		return;
	}

	PlayerMoveState = PlayerPawn->GetMoveState();
	bHasThrowable = PlayerPawn->GetHasThrowable();
}

void UPlayerAnimation::HandleThrow()
{
	if (IsValid(ThrowMontage) && !Montage_IsPlaying(ThrowMontage))
	{
		Montage_Play(ThrowMontage);
	}
}
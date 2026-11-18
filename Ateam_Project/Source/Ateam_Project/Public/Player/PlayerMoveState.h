
#pragma once

#include "CoreMinimal.h"
#include "PlayerMoveState.generated.h"

UENUM(BlueprintType)
enum class EPlayerMoveState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Dash UMETA(DisplayName = "Dash"),
};
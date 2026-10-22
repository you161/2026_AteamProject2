
#pragma once

#include "CoreMinimal.h"
#include "ThrowableState.generated.h"

/**
 *アイテムを投げる際の状態を表すState
 */
UENUM(BlueprintType)

enum class EThrowableState : uint8
{
    Pickable    UMETA(DisplayName = "Pickable"),
    Held    UMETA(DisplayName = "Held"),
    Thrown  UMETA(DisplayName = "Thrown"),
    Homing  UMETA(DisplayName = "Homing")
};
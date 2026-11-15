#pragma once

#include "CoreMinimal.h"

//ダメージ対象のキャラクター種別
enum class ECharacterType : uint8
{
	Player = 0,
	Enemy = 1,
	//種類数
	Count
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoundData.generated.h"

/**
 * 音素材を管理するデータアセット
 */
UENUM(BlueprintType)
enum class ESEType : uint8
{
    ObjectCollect  UMETA(DisplayName = "オブジェクト回収"),
    ObjectThrow    UMETA(DisplayName = "オブジェクト投擲"),
    PlayerWalk   UMETA(DisplayName = "プレイヤーの足音"),
    PlayerDash   UMETA(DisplayName = "プレイヤーのダッシュ"),
    PlayerAttack   UMETA(DisplayName = "プレイヤー攻撃"),
    PlayerDamage   UMETA(DisplayName = "プレイヤーダメージ"),
    EnemyDamage    UMETA(DisplayName = "敵ダメージ"),
    EnemyDamage2   UMETA(DisplayName = "敵ダメージ(2個)"),
    EnemyDamage3   UMETA(DisplayName = "敵ダメージ(3個)"),
};

UENUM(BlueprintType)
enum class EBGMType : uint8
{
    None      UMETA(DisplayName = "なし"),
    Title      UMETA(DisplayName = "タイトル"),
    Main       UMETA(DisplayName = "メイン"),
    Clear      UMETA(DisplayName = "クリア"),
    GameOver   UMETA(DisplayName = "ゲームオーバー"),
};

USTRUCT(BlueprintType)
struct FSoundEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USoundBase> Sound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
    float Volume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Pitch = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bLoop = false;
};

UCLASS(BlueprintType)
class ATEAM_PROJECT_API USoundData : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "BGM")
    TMap<EBGMType, FSoundEntry> BGMMap = {};

    UPROPERTY(EditAnywhere, Category = "SE")
    TMap<ESEType, FSoundEntry> SEMap = {};
};
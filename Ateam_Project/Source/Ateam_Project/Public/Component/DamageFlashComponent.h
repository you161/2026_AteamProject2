
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageFlashComponent.generated.h"

class UMaterialInstanceDynamic;

//ダメージを受けた時にメッシュを発光させて点滅させるコンポーネント
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATEAM_PROJECT_API UDamageFlashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// コンストラクタ。
	UDamageFlashComponent();

	//点滅を開始する
	UFUNCTION(BlueprintCallable, Category = "DamageFlash")
	void PlayFlash();

protected:
	//ゲーム開始時にダイナミックマテリアルを作成する
	virtual void BeginPlay() override;
	//点滅中の値を減衰させる
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

private:
	//全ダイナミックマテリアルに点滅の値を設定する
	void SetFlashValue(float Value);

private:
	//点滅の長さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFlash", meta = (AllowPrivateAccess = "true"))
	float FlashDuration = 1.0f;
	//点滅の速さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFlash", meta = (AllowPrivateAccess = "true", ClampMin = "0.1"))
	float FlashSpeed = 5.0f;
	//点滅色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFlash", meta = (AllowPrivateAccess = "true"))
	FLinearColor FlashColor = FLinearColor::Red;
	//マテリアル側の点滅強度パラメータ名
	UPROPERTY(EditAnywhere, Category = "DamageFlash", meta = (AllowPrivateAccess = "true"))
	FName FlashParamName = TEXT("DamageFlash");
	//マテリアル側の点滅色パラメータ名
	UPROPERTY(EditAnywhere, Category = "DamageFlash", meta = (AllowPrivateAccess = "true"))
	FName FlashColorParamName = TEXT("FlashColor");
	//作成したダイナミックマテリアルの一覧
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials = {};
	//点滅開始からの経過時間
	float Elapsed = 0.0f;
	//点滅中かどうか
	bool bIsPlaying = false;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitPointBar.generated.h"

class UCanvasPanel;
class UImage;
class UHitPointBar;

/**
 *   HPバーの処理
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPBarEmptied, UHitPointBar*);
UCLASS()
class ATEAM_PROJECT_API UHitPointBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void UpdateHitPointBar(float MaxHP, float Damege);

    UPROPERTY(meta = (BindWidgetOptional))
    class UCanvasPanel* CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    UImage* currentBar;

    UPROPERTY(meta = (BindWidget))
    UImage* healBar;

    UPROPERTY(meta = (BindWidget))
    UImage* damageBar;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "aaa")
    float MoveSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "aaa")
    float ShakeRange = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "aaa")
    float ShakeTime = 1.0f;

    FOnHPBarEmptied OnHPBarEmptied;   //バーが空になりきったら通知

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    FVector2D currentSize = FVector2D(1.0f, 1.0f);
    FVector2D healSize = FVector2D(1.0f, 1.0f);
    FVector2D damageSize = FVector2D(1.0f, 1.0f);
    float countTime = 0.0f;
    bool bIsImageFound = false;
    bool bIsShake = false;

    bool bIsEmptyNotified = false;    //通知済みフラグ
};

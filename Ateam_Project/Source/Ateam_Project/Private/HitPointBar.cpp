// Fill out your copyright notice in the Description page of Project Settings.


#include "HitPointBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void UHitPointBar::NativeConstruct()
{
    Super::NativeConstruct();

    bIsImageFound = true;
    if (!CanvasPanel)
    {
        UE_LOG(LogTemp, Warning, TEXT("CanvasPanel is not set in HitPointBar."));
        bIsImageFound = false;
    }
    if (!currentBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("currentBar is not set in HitPointBar."));
        bIsImageFound = false;
    }
    if (!healBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("healBar is not set in HitPointBar."));
        bIsImageFound = false;
    }
    if (!damageBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("damageBar is not set in HitPointBar."));
        bIsImageFound = false;
    }

    if (bIsImageFound)
    {
        currentSize = currentBar->GetRenderTransform().Scale;
        healSize = healBar->GetRenderTransform().Scale;
        damageSize = damageBar->GetRenderTransform().Scale;
    }
    countTime = 0.0f;
    bIsShake = false;
    bIsEmptyNotified = false;
}

void UHitPointBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!bIsImageFound) return;


    if (bIsShake)
    {
        countTime += InDeltaTime;

        FVector2D CanvLocation = GetRenderTransform().Translation;

        CanvLocation.X = FMath::RandRange(-ShakeRange, ShakeRange);
        CanvLocation.Y = FMath::RandRange(-ShakeRange, ShakeRange);

        if (countTime > ShakeTime)
        {
            CanvLocation.ZeroVector;
            bIsShake = false;
        }

        CanvasPanel->SetRenderTranslation(CanvLocation);
        return;
    }

    // HPが減ったら
    if (damageSize.X > currentSize.X)
    {
        damageSize.X = FMath::Max(currentSize.X, damageSize.X - MoveSpeed * InDeltaTime);
        damageBar->SetRenderScale(damageSize);

        //縮み終わった瞬間 かつ HPが0なら、一度だけ通知
        if (damageSize.X <= currentSize.X && currentSize.X <= 0.0f && !bIsEmptyNotified)
        {
            bIsEmptyNotified = true;
            OnHPBarEmptied.Broadcast(this);
        }
    }
    
    // HPが増えたら
    if (healSize.X > currentSize.X)
    {
        currentSize.X = FMath::Min(healSize.X, currentSize.X - MoveSpeed * InDeltaTime);
        currentBar->SetRenderScale(currentSize);
    }
}

void UHitPointBar::UpdateHitPointBar(float MaxHP, float Damage)
{
    if (!bIsImageFound)
    {
        UE_LOG(LogTemp, Warning, TEXT("HitPointBar is failing as a component."))
        return;
    }

    float param = FMath::Min(1.0f, FMath::Max(0.0f, Damage / MaxHP));

    // HPが減る
    if (currentSize.X > 1.0f - param)
    {
        bIsShake = true;
        currentSize.X = 1.0f - param;
        currentBar->SetRenderScale(currentSize);
    }
    // HPが増える
    else
    {
        damageSize.X = 1.0f - param;
        damageBar->SetRenderScale(damageSize);
    }
    healSize.X = 1.0f - param;
    healBar->SetRenderScale(healSize);
}
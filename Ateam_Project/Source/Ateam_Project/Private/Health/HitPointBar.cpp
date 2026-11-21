// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/HitPointBar.h"
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

        FVector2D CanvLocation = FVector2D::ZeroVector;
        if (countTime <= ShakeTime)
        {
            CanvLocation.X = FMath::RandRange(-ShakeRange, ShakeRange);
            CanvLocation.Y = FMath::RandRange(-ShakeRange, ShakeRange);
        }
        else
        {
            bIsShake = false;
        }
        CanvasPanel->SetRenderTranslation(CanvLocation);
    }

    //ダメージバーを追従させる
    if (!bIsShake && damageSize.X > currentSize.X)
    {
        damageSize.X = FMath::Max(currentSize.X, damageSize.X - MoveSpeed * InDeltaTime);
        damageBar->SetRenderScale(damageSize);
    }

    //空になりきったか
    if (!bIsEmptyNotified && currentSize.X <= 0.0f && damageSize.X <= 0.0f)
    {
        bIsEmptyNotified = true;
        OnHPBarEmptied.Broadcast(this);
    }

    //回復処理
    if (healSize.X > currentSize.X)
    {
        currentSize.X = FMath::Min(healSize.X, currentSize.X + MoveSpeed * InDeltaTime);
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

    const float Target = 1.0f - param;

    //HPが減る
    if (currentSize.X > Target)
    {
        countTime = 0.0f;
        bIsShake = true;
        currentSize.X = Target;
        currentBar->SetRenderScale(currentSize);
    }
    //HPが増える
    else if (currentSize.X < Target)
    {
        damageSize.X = Target;
        damageBar->SetRenderScale(damageSize);
    }

    //同値なら何もしない
    healSize.X = Target;
    healBar->SetRenderScale(healSize);
}
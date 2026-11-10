// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effect/EffectData.h"
#include "EffectManager.generated.h"

UCLASS()
class ATEAM_PROJECT_API AEffectManager : public AActor
{
	GENERATED_BODY()
	
public:
	AEffectManager();

	void PlayEffect(EEffectType Type, FVector Location);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EffectData")
	TObjectPtr<UEffectData> EffectData = nullptr;

private:

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

class UEnemyData;
class UBoxComponent;

UCLASS()
class ATEAM_PROJECT_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 敵のアセットデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyData")
	TObjectPtr<UEnemyData> EnemyData = nullptr;

	// プレイヤーのアクター
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerPawn")
	TObjectPtr<AActor> PlayerPawn = nullptr;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	TObjectPtr<UBoxComponent> BoxCollision;
	float countTime = 0.0f;
	bool bIsAttack = false;

};

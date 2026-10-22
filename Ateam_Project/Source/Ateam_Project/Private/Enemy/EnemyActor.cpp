// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyData.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in EnemyActor"));
	}
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in EnemyActor."));
	}
	if (EnemyData->BP_FallActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_AttackActor is not set in EnemyActor."));
	}

	BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision != nullptr)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	countTime = 0.0f;
	bIsAttack = false;
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	countTime += DeltaTime;

	if (countTime > EnemyData->CooldownTime)
	{
		for (int i = 0; i < EnemyData->ActorCount; ++i)
		{
			double index = i % EnemyData->SetArea.Num();
			double minX = EnemyData->SetArea[index].X;
			double minY = EnemyData->SetArea[index].Y;
			double maxX = EnemyData->SetArea[index].Z;
			double maxY = EnemyData->SetArea[index].W;
			GetWorld()->SpawnActor<AActor>(EnemyData->BP_FallActor,
				FTransform(FVector(FMath::RandRange(minX, maxX), FMath::RandRange(minY, maxY), EnemyData->SetHeight)));
		}
		countTime = 0.0f;
	}
}

void AEnemyActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap was called by EnemyActor."));

	}
}
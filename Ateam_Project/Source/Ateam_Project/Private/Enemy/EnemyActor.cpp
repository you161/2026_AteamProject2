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
	if (BP_AttackActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_AttackActor is not set in EnemyActor."));
	}
	
	bIsAttack = false;
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsAttack)
	{
		countTime += DeltaTime;

		if (countTime > EnemyData->CooldownTime)
		{
			AttackActor.Empty();
			for (int i = 0; i < EnemyData->ActorCount; ++i)
			{
				AttackActor.Emplace();
				AttackActor[i] = GetWorld()->SpawnActor<AActor>(BP_AttackActor, FTransform(
					FVector(FMath::RandRange(-1000.f, 1000.f), FMath::RandRange(-1000.f, 1000.f), 1000.f)));
				AttackCollsition.Emplace();
				AttackCollsition[i] = AttackActor[i]->FindComponentByClass<UBoxComponent>();
				AttackCollsition[i]->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
			}
			countTime = 0.0f;
			bIsAttack = true;
		}
	}
	else if (bIsAttack)
	{
		countTime += DeltaTime;
		if (AttackActor.Num() == 0) return;
		for (int i = AttackActor.Num() - 1; i >= 0; --i)
		{
			FVector currentLocation = AttackActor[i]->GetActorLocation();
			currentLocation.Z += EnemyData->MoveSpeed * 0.5 * 9.8 * DeltaTime * DeltaTime;
			AttackActor[i]->SetActorLocation(currentLocation);
		}
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
		UE_LOG(LogTemp, Warning, TEXT("Overlap!"));

		AActor* HitAttackActor = OverlappedComp->GetOwner();
		if (HitAttackActor)
		{
			int32 Index = AttackActor.Find(HitAttackActor);
			if (Index != INDEX_NONE && IsValid(AttackActor[Index]))
			{
				AttackActor[Index]->Destroy();

				AttackActor.RemoveAt(Index);
				AttackCollsition.RemoveAt(Index);

				if (AttackActor.Num() == 0)
				{
					countTime = 0.0f;
					bIsAttack = false;
				}
			}
		}
	}
}


// 記念品
void AEnemyActor::MoveCurve(float DeltaTime)
{
	if (FMath::Cos(theta) < 1 && bIsAttack)
	{
		theta += 0.01f;
		TargetDirection = PlayerPawn->GetActorLocation() - AttackActor[0]->GetActorLocation();
		TargetDirection.Normalize();
		bIsAttack = false;
	}
	else if (bIsAttack)
	{

	}

	FVector currentLocation = AttackActor[0]->GetActorLocation();
	currentLocation += TargetDirection * EnemyData->MoveSpeed * DeltaTime;
	currentLocation.Y = currentLocation.Y + FMath::Cos(theta) * 25.f;
	AttackActor[0]->SetActorLocation(currentLocation);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Enemy/FallActor.h"
#include "Enemy/EnemyData.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	SetEnemyData();
	SetBoxCollision();
	RcountTime = 0.0f;
	HcountTime = 0.0f;
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsConfigured) return;

	RcountTime += DeltaTime;
	HcountTime += DeltaTime;

	if (EnemyData->bHasRandom)
	{
		SpawnRandom();
	}

	if (EnemyData->bHasHoming)
	{
		SpawnHoming();
	}
}

void AEnemyActor::SetEnemyData()
{
	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in EnemyActor"));
		bIsConfigured = false;
	}
	else
	{
		bIsConfigured = EnemyData->IsConfigured();
	}
}

void AEnemyActor::SetBoxCollision()
{
	BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision != nullptr)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BoxCollision is not set in EnemyActor"));
		bIsConfigured = false;
	}
}

void AEnemyActor::SpawnRandom()
{
	if (RcountTime > EnemyData->RCooldownTime)
	{
		for (int i = 0; i < EnemyData->ActorCount; ++i)
		{
			double index = i % EnemyData->RSetArea.Num();
			double minX = EnemyData->RSetArea[index].X;
			double minY = EnemyData->RSetArea[index].Y;
			double maxX = EnemyData->RSetArea[index].Z;
			double maxY = EnemyData->RSetArea[index].W;
			AFallActor* RandomActor = GetWorld()->SpawnActor<AFallActor>(EnemyData->RFallActor,
				FTransform(FVector(FMath::RandRange(minX, maxX), FMath::RandRange(minY, maxY), EnemyData->RFallHeight)));
			RandomActor->SetHoming(false);
		}
		RcountTime = 0.0f;
	}
}

void AEnemyActor::SpawnHoming()
{
	if (HcountTime > EnemyData->HCooldownTime)
	{
		AActor* PlayerPawn = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyData->PlayerPawn);
		AFallActor* HomingActor = GetWorld()->SpawnActor<AFallActor>(EnemyData->HFallActor,
			FTransform(FVector(PlayerPawn->GetActorLocation().X, PlayerPawn->GetActorLocation().Y, EnemyData->HFallHeight)));
		HomingActor->SetHoming(true);
		HcountTime = 0.0f;
		FActorSpawnParameters sp;
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
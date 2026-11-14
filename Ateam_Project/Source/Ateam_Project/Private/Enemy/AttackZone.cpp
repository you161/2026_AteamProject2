// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AttackZone.h"
#include "Components/SphereComponent.h"
#include "Enemy/EnemyActor.h"

AAttackZone::AAttackZone()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AAttackZone::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<AEnemyActor> EnemyActor = Cast<AEnemyActor>(GetOwner());

	SphereComp = FindComponentByClass<USphereComponent>();
	if (IsValid(SphereComp))
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAttackZone::OnOverlapBegin);
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &AAttackZone::OnOverlapEnd);
	}

	bIsHitPlayer = false;
	bIsValid = true;
	if (IsValid(EnemyActor))
	{
		ZoneSize = EnemyActor->GetAttackZoneSize();
		MaxActorSize = FVector(ZoneSize, ZoneSize, 1.0f);
		MaxSphereRadius = SphereComp->GetScaledSphereRadius() * ZoneSize;

		SphereComp->SetRelativeScale3D(FVector::Zero());

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAttackZone::TickZone, 0.01f, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor is not set owner in AttackZone."));
		bIsValid = false;
	}
}

void AAttackZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttackZone::TickZone()
{
	countTime += 0.01f;

	float Alpha = FMath::Clamp(countTime / SetTime, 0.0f, 1.0f);

	FVector NewActorScale = FMath::Lerp(0.0f, MaxActorSize, Alpha);
	float NewSphereRadius = FMath::Lerp(0.0f, MaxSphereRadius, Alpha);

	SetActorScale3D(NewActorScale);
	SphereComp->SetSphereRadius(NewSphereRadius);

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void AAttackZone::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepRsult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherComp->ComponentHasTag(TEXT("Player")))
		{
			bIsHitPlayer = true;
		}
	}
}

void AAttackZone::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherComp->ComponentHasTag(TEXT("Player")))
		{
			bIsHitPlayer = false;
		}
	}
}

bool AAttackZone::GetIsHitPlayer()
{
	return bIsHitPlayer;
}
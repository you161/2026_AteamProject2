// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FallActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyActor.h"
#include "Enemy/AttackZone.h"
#include "Health/HealthManager.h"

AFallActor::AFallActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFallActor::BeginPlay()
{
	Super::BeginPlay();

	EnemyActor = Cast<AEnemyActor>(GetOwner());

	SetBoxCollision();

	MoveActive();
	
	FVector StartLocation = GetActorLocation();
	StartLocation.Z = FallHeight;
	SetActorLocation(StartLocation);

	countTime = 0.0f;
	FallTime = 0.0f;

	HealthManager = Cast<AHealthManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AHealthManager::StaticClass()));
}

void AFallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	countTime += DeltaTime;

	Falling(DeltaTime);
}

void AFallActor::SetBoxCollision()
{
	BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision != nullptr)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallActor::OnOverlapBegin);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BoxCollision is not set in FallActor"));
	}
}

void AFallActor::Falling(float DeltaTime)
{
	double size = FMath::Clamp(countTime / WiteTime, 0.0f, 1.0f);

	double actorSize = FMath::Max(size - 0.3, 0.0f) * 2.0;
	if (actorSize >= 1.0) actorSize = 1.0;
	else if (actorSize >= 0.9) actorSize = 0.9;
	else if (actorSize >= 0.8) actorSize = 0.8;
	else if (actorSize >= 0.7) actorSize = 0.7;

	SetActorScale3D(FVector(MaxActorSize, MaxActorSize, MaxActorSize) * actorSize);

	FVector currentLocation = GetActorLocation();

	if (countTime > WiteTime)
	{
		FallTime += DeltaTime;
		currentLocation.Z -= FallSpeed * 0.5f * 9.80665f * FallTime * FallTime;
	}

	SetActorLocation(currentLocation);
}

UFUNCTION()
void AFallActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherComp->ComponentHasTag(TEXT("Player")))
		{
			if (IsValid(HealthManager))
			{
				HealthManager->TakeDamage(ECharacterType::Player);
			}


			ZoneActor->Destroy();
			Destroy();
		}

		if (OtherComp->ComponentHasTag(TEXT("Zone")))
		{
			if (ZoneActor->GetIsHitPlayer())
			{
				if (IsValid(HealthManager))
				{
					HealthManager->TakeDamage(ECharacterType::Player);
				}
			}
			ZoneActor->Destroy();
			Destroy();
		}
	}
}

void AFallActor::MoveActive()
{
	if (IsValid(EnemyActor))
	{
		ZoneActor = EnemyActor->GetZoneActor();
		MaxActorSize = EnemyActor->GetAttackActorSize();
		FallSpeed = EnemyActor->GetAttackMoveSpeed();
	}
}
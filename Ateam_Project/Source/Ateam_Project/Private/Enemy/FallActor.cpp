// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FallActor.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyData.h"

AFallActor::AFallActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFallActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in AFallActor"));
	}

	BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision != nullptr)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallActor::OnOverlapBegin);
	}
	countTime = 0.0f;
}

void AFallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	countTime += DeltaTime;
	FVector currentLocation = GetActorLocation();
	currentLocation.Z -= EnemyData->MoveSpeed * 0.5f * 9.80665f * countTime * countTime;
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
		// UE_LOG(LogTemp, Warning, TEXT("Overlap was called by FallActor"));
		Destroy();
	}
}
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
	
	bIsAttack = true;
	AttackActor = GetWorld()->SpawnActor<AActor>(BP_AttackActor, FTransform(GetActorLocation() + AttackActorLocation));
	AttackCollsition = AttackActor->FindComponentByClass<UBoxComponent>();
	if (AttackCollsition != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello World"));
		AttackCollsition->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no component AttackCollision"));
	}
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackActor == nullptr) return;

	if (bIsAttack)
	{
		TargetDirection = PlayerPawn->GetActorLocation() - AttackActor->GetActorLocation();
		TargetDirection.Normalize();
		bIsAttack = false;
	}
	FVector DeltaMove = TargetDirection * EnemyData->AttackSpeed * DeltaTime;
	FHitResult HitResult;
	AttackActor->AddActorWorldOffset(DeltaMove, true, &HitResult);
}

void AEnemyActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit OtherActor"));

	if (OtherActor && OtherActor != this)
	{
		AttackActor->Destroy();
		AttackActor = nullptr;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SubEnemy.h"
#include "Enemy/EnemyData.h"
#include "Kismet/GameplayStatics.h"

ASubEnemy::ASubEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASubEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(EnemyData))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in SubEnemy."));
		bIsConfigured = false;
	}

	if (bIsConfigured)
	{
		SetPlayerPawn();
	}
	
}

void ASubEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsConfigured) return;

	FVector direction = PlayerPawn->GetActorLocation() - GetActorLocation();
	direction.Normalize();
	AddActorLocalOffset(direction * MoveSpeed * DeltaTime);
}

void ASubEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASubEnemy::SetPlayerPawn()
{
	//PlayerPawn = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyData->PlayerPawn);
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in FallActor"));
		bIsConfigured = false;
	}
}
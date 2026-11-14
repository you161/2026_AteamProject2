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

	SetObjects();
	SetBoxCollision();
	ResetAttack();
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsValid == false) return;

	if (!bIsReady)
	{
		ReadyTime += DeltaTime;
		if (ReadyTime > EnemyData->SpawnTime)
		{
			bIsReady = true;
		}
	}
	else
	{
		SetAttackActor();

		TotalTime += DeltaTime;
		SpawnAttackActor();
	}
}

void AEnemyActor::SetObjects()
{
	bIsValid = true;

	if (!IsValid(EnemyData))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in EnemyActor."));
		bIsValid = false;
	}

	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in EnemyActor."));
		bIsValid = false;
	}

	if (!IsValid(TurnMoveActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TurnMoveActor is not set in EnemyActor."));
		bIsValid = false;
	}

	if (!IsValid(AttackZone))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackZone is not set in EnemyActor."));
		bIsValid = false;
	}
}

void AEnemyActor::SetBoxCollision()
{
	TArray<UBoxComponent*> BoxComp;
	GetComponents<UBoxComponent>(BoxComp);
	for (UBoxComponent* Comp : BoxComp)
	{
		if (Comp && Comp->ComponentHasTag(FName("Head")))
		{
			HeadCollision = Comp;
		}
		if (Comp && Comp->ComponentHasTag(FName("Body")))
		{
			BodyCollision = Comp;
		}
		if (Comp && Comp->ComponentHasTag(FName("Leg")))
		{
			LegCollision = Comp;
		}
	}

	if (!IsValid(HeadCollision))
	{
		UE_LOG(LogTemp, Warning, TEXT("HeadCollision is not set in EnemyActor"));
		bIsValid = false;
	}

	if (!IsValid(BodyCollision))
	{
		UE_LOG(LogTemp, Warning, TEXT("BodyCollision is not set in EnemyActor"));
		bIsValid = false;
	}

	if (!IsValid(LegCollision))
	{
		UE_LOG(LogTemp, Warning, TEXT("LegCollision is not set in EnemyActor"));
		bIsValid = false;
	}

	if (!bIsValid)
	{
		HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
		BodyCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
		LegCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
}

void AEnemyActor::SetAttackActor()
{
	int32 Range = FMath::Min(EnemyData->Count, AttackArray.Num());
	for (int32 index = TurnMoveArray.Num(); index < Range; ++index)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FVector AttackLocation = FVector::Zero();
		AttackLocation.Y = EnemyData->AttackDistance.X + EnemyData->AttackDistance.Y * (index / 2);
		if (index % 2 == 1)
		{
			AttackLocation.Y *= -1;
		}
		AttackLocation.Z = FMath::RandRange(0.0, EnemyData->AttackDistance.Z);
		TurnMoveArray.Add(GetWorld()->SpawnActor<ATurnMoveActor>(TurnMoveActor, FTransform(GetActorLocation() + AttackLocation), SpawnParams));
	}
}

void AEnemyActor::ResetAttack()
{
	bIsReady = false;
	ReadyTime = 0.0f;
	TotalTime = 0.0f;
	AttackArray = EnemyData->AttackState;
	TurnMoveArray.Reset();
}

void AEnemyActor::SpawnAttackActor()
{
	TArray<int32> IndexToRemove;

	AttackArray.RemoveAll([this](const FAttackState& Item)
	{
		if (TotalTime > Item.AttackTime)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			AttackActorSize = Item.ActorSize;
			AttackZoneSize = Item.ZoneSize;
			AttackMoveSpeed = Item.MoveSpeed;
			AttackRotationSpeed = Item.RotationSpeed;

			ActorRotator.Pitch = FMath::RandRange(Item.PitchRange.X, Item.PitchRange.Y);
			ActorRotator.Yaw = FMath::RandRange(Item.YawRange.X, Item.YawRange.Y);
			if (TurnMoveArray[0]->GetActorLocation().Y < GetActorLocation().Y)
			{
				ActorRotator.Yaw *= -1;
			}

			FVector ZoneLocation = FVector::Zero();
			switch (Item.AttackSeed)
			{
			case EAttackSeed::RandomAttack:
				ZoneLocation.X = FMath::RandRange(Item.SpawnArea.X, Item.SpawnArea.Z);
				ZoneLocation.Y = FMath::RandRange(Item.SpawnArea.Y, Item.SpawnArea.W);
				ZoneLocation.Z = GroundHeight;
				ZoneActor = GetWorld()->SpawnActor<AAttackZone>(AttackZone, FTransform(ZoneLocation), SpawnParams);
				break;

			case EAttackSeed::HomingAttack:
				ZoneLocation = FVector(PlayerPawn->GetActorLocation().X, PlayerPawn->GetActorLocation().Y, GroundHeight);
				ZoneActor = GetWorld()->SpawnActor<AAttackZone>(AttackZone, FTransform(ZoneLocation), SpawnParams);
				break;
			}

			TurnMoveArray[0]->MoveActive();
			TurnMoveArray.RemoveAt(0);

			return true;
		}

		return false;
	});
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
		if (OverlappedComp == HeadCollision)
		{
			// 頭に当たった時の処理

		}

		if (OverlappedComp == BodyCollision)
		{
			// 体に当たった時の処理

		}

		if (OverlappedComp == LegCollision)
		{
			// 足に当たった時の処理

		}
	}
}

TObjectPtr<AAttackZone> AEnemyActor::GetZoneActor()
{
	return ZoneActor;
}

FRotator AEnemyActor::GetActorRotator()
{
	return ActorRotator;
}

float AEnemyActor::GetAttackActorSize()
{
	return AttackActorSize;
}

float AEnemyActor::GetAttackZoneSize()
{
	return AttackZoneSize;
}

float AEnemyActor::GetAttackMoveSpeed()
{
	return AttackMoveSpeed;
}

float AEnemyActor::GetAttackRotationSpeed()
{
	return AttackRotationSpeed;
}
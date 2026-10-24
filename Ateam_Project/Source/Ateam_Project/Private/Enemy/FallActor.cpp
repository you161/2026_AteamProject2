// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FallActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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

	if (EnemyData->IsConfigured())
	{
		PlayerPawn = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyData->PlayerPawn);

		// 床が見つかるまでましたを確認する
		FVector StartTrace = GetActorLocation();
		FVector EndTrace = StartTrace - FVector(0.f, 0.f, 10000.f);
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(PlayerPawn);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			ZoneActor = GetWorld()->SpawnActor<AActor>(EnemyData->DamageZone, FTransform(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.ImpactPoint.Z)));
		}
		else
		{
			ZoneActor = GetWorld()->SpawnActor<AActor>(EnemyData->DamageZone, FTransform(FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f)));
		}
		if (ZoneActor != nullptr)
		{
			ZoneMesh = ZoneActor->FindComponentByClass<UStaticMeshComponent>();
			ZoneCollision = ZoneActor->FindComponentByClass<UCapsuleComponent>();
			BaseMesh = ZoneMesh->GetRelativeScale3D();
			BaseCollision = ZoneCollision->GetRelativeScale3D();
			if (ZoneCollision != nullptr)
			{
				ZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallActor::OnOverlapBegin);
				ZoneCollision->OnComponentEndOverlap.AddDynamic(this, &AFallActor::OnOverlapEnd);
			}

		}
	}

	BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision != nullptr)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallActor::OnOverlapBegin);
	}
	countTime = 0.0f;
	FallTime = 0.0f;
	bIsReady = true;
	bIsFalling = false;
	bIsHoming = false;
	bIsHitPlayer = false;
}

void AFallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	countTime += DeltaTime;
	double size = 1.0;
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FVector ZoneLocation = ZoneActor->GetActorLocation();
	FVector currentLocation = GetActorLocation();
	if (bIsHoming)
	{
		if (bIsReady)
		{
			size = FMath::Min(countTime / EnemyData->HWarningTime, 1.0);
			ZoneLocation.X = PlayerLocation.X;
			ZoneLocation.Y = PlayerLocation.Y;
			currentLocation.X = currentLocation.X;
			currentLocation.Y = currentLocation.Y;
			if (countTime > EnemyData->HWarningTime)
			{
				bIsReady = false;
				bIsFalling = true;
			}
		}
		if (bIsFalling)
		{
			FallTime += DeltaTime;
			currentLocation.Z -= EnemyData->HFallSpeed * 0.5f * 9.80665f * FallTime * FallTime;
		}
	}
	else
	{
		size = FMath::Min(countTime / EnemyData->RWarningTime, 1.0f);
		if (countTime > EnemyData->RWarningTime)
		{
			FallTime += DeltaTime;
			currentLocation.Z -= EnemyData->RFallSpeed * 0.5f * 9.80665f * FallTime * FallTime;
		}
	}
	ZoneMesh->SetRelativeScale3D(size* BaseMesh);
	ZoneCollision->SetRelativeScale3D(size* BaseCollision);
	ZoneActor->SetActorRelativeLocation(ZoneLocation);
	SetActorLocation(currentLocation);
}

void AFallActor::SetHoming()
{
	bIsHoming = true;
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
		if (OverlappedComp == ZoneCollision && OtherActor->IsA(EnemyData->PlayerPawn))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name: %s / Class: %s"),
				*OtherActor->GetName(),
				*OtherActor->GetClass()->GetName());
			bIsHitPlayer = true;
		}
		else if (OverlappedComp == BoxCollision && OtherComp == ZoneMesh)
		{
			if (bIsHitPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("It hit the player!"));
			}
			ZoneActor->Destroy();
			Destroy();
		}
	}
}

void AFallActor::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (OverlappedComp == ZoneCollision && OtherActor->IsA(EnemyData->PlayerPawn))
		{
			bIsHitPlayer = false;
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FallActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyData.h"
#include "Health/HealthManager.h"

AFallActor::AFallActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFallActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetEnemyData();
	SetBoxCollision();
	if (bIsConfigured)
	{
		SetPlayerPawn();
	}
	countTime = 0.0f;
	FallTime = 0.0f;
	bIsHoming = false;
	bIsHitPlayer = false;

	HealthManager = Cast<AHealthManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AHealthManager::StaticClass()));
}

void AFallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsConfigured) return;

	countTime += DeltaTime;

	float warningTime = EnemyData->RWarningTime;
	float fallSpeed = EnemyData->RFallSpeed;
	if (bIsHoming)
	{
		warningTime = EnemyData->HWarningTime;
		fallSpeed = EnemyData->HFallSpeed;
	}

	UpdateSize(warningTime);
	UpdateLocation(DeltaTime, warningTime, fallSpeed);
	
	
}

void AFallActor::SetEnemyData()
{
	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in AFallActor"));
		bIsConfigured = false;
	}
	else
	{
		bIsConfigured = EnemyData->IsConfigured();
	}
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

void AFallActor::SetPlayerPawn()
{
	PlayerPawn = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyData->PlayerPawn);
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in FallActor"));
		bIsConfigured = false;
	}
}

void AFallActor::SetZoneMesh()
{
	
	ZoneMesh = ZoneActor->FindComponentByClass<UStaticMeshComponent>();
	if (ZoneMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ZoneMesh is not set in AFallActor"));
		bIsConfigured = false;
	}
}

void AFallActor::SetZoneCollision()
{
	ZoneCollision = ZoneActor->FindComponentByClass<UCapsuleComponent>();
	if (ZoneCollision != nullptr)
	{
		ZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &AFallActor::OnOverlapBegin);
		ZoneCollision->OnComponentEndOverlap.AddDynamic(this, &AFallActor::OnOverlapEnd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ZoneCollision is not set in AFallActor"));
		bIsConfigured = false;
	}
}

void AFallActor::SetBaseSize()
{
	BaseSizeActor = GetActorScale3D();
	if (bIsConfigured)
	{
		BaseSizeMesh = ZoneMesh->GetRelativeScale3D();
		BaseSizeColl = ZoneCollision->GetRelativeScale3D();
	}
	else
	{
		BaseSizeMesh = FVector(0.0f, 0.0f, 0.0f);
		BaseSizeColl = FVector(0.0f, 0.0f, 0.0f);
	}
}

void AFallActor::SpawnZoneActor()
{
	auto zone = EnemyData->RandomZone;
	if (bIsHoming)
	{
		zone = EnemyData->HomingZone;
	}
	// 床が見つかるまでましたを確認する
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = StartTrace - FVector(0.f, 0.f, 10000.f);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(PlayerPawn);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		StartTrace.Z = HitResult.ImpactPoint.Z;
		ZoneActor = GetWorld()->SpawnActor<AActor>(zone, FTransform(StartTrace));
	}
	else
	{
		StartTrace.Z = 0.0;
		ZoneActor = GetWorld()->SpawnActor<AActor>(zone, FTransform(StartTrace));
	}
	if (ZoneActor != nullptr)
	{
		SetZoneMesh();
		SetZoneCollision();
	}
}

void AFallActor::UpdateSize(float WaringTime)
{
	double size = FMath::Min(countTime / WaringTime, 1.0);

	double actorSize = FMath::Max(size - 0.3, 0.0f) * 2.0;
	if (actorSize >= 1.0) actorSize = 1.0;
	else if (actorSize >= 0.9) actorSize = 0.9;
	else if (actorSize >= 0.8) actorSize = 0.8;
	else if (actorSize >= 0.7) actorSize = 0.7;

	SetActorScale3D(actorSize * BaseSizeActor);
	ZoneMesh->SetRelativeScale3D(size * BaseSizeMesh);
	ZoneCollision->SetRelativeScale3D(size * BaseSizeColl);
}

void AFallActor::UpdateLocation(float DeltaTime, float WarningTime, float FallSpeed)
{
	FVector currentLocation = GetActorLocation();
	FVector ZoneLocation = ZoneActor->GetActorLocation();

	if (bIsHoming)
	{
		FVector direction = PlayerPawn->GetActorLocation() - currentLocation;
		FVector Zdirection = PlayerPawn->GetActorLocation() - ZoneLocation;
		direction.Z = 0.0f;
		Zdirection.Z = 0.0f;
		direction.Normalize();
		Zdirection.Normalize();
		currentLocation += EnemyData->HomingSpeed * direction;
		ZoneLocation += EnemyData->HomingSpeed * Zdirection;
		if (countTime > WarningTime)
		{
			bIsHoming = false;
		}
	}

	if (countTime > WarningTime)
	{
		FallTime += DeltaTime;
		currentLocation.Z -= FallSpeed * 0.5f * 9.80665f * FallTime * FallTime;
	}

	SetActorLocation(currentLocation);
	ZoneActor->SetActorRelativeLocation(ZoneLocation);
}

void AFallActor::SetHoming(bool flag)
{
	bIsHoming = flag;
	SpawnZoneActor();
	SetBaseSize();
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
				if (IsValid(HealthManager))
				{
					HealthManager->TakeDamage(CharactorType::Player);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("HealthManager is null in AFallActor::OnOverlapBegin"));
				}

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
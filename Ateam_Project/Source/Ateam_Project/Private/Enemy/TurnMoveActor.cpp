// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TurnMoveActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy/EnemyActor.h"
#include "Enemy/AttackZone.h"
#include "Enemy/EnemyManager.h"

ATurnMoveActor::ATurnMoveActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComp->SetCollisionProfileName(TEXT("Trigger"));

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATurnMoveActor::OnOverlapBegin);

}

void ATurnMoveActor::BeginPlay()
{
	Super::BeginPlay();

	EnemyActor = Cast<AEnemyActor>(GetOwner());

	BaseLocation = GetActorLocation();
	StartLocation = BaseLocation;

	TurnMoveState = ETurnMoveState::Spawn;
	StartSize = GetActorScale3D();
	bIsMove = false;

	bIsValid = true;
	EnemyManager = Cast<AEnemyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass()));
	if (!IsValid(EnemyManager))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManager is null in ATurnMoveActor"));
	}
}

void ATurnMoveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsValid) return;

	countTime += DeltaTime;

	FVector Size;
	float Alpha;

	switch (TurnMoveState)
	{
	case ETurnMoveState::Spawn:
		MoveRoll();
		break;

	case ETurnMoveState::Ready:
		MoveDrift();
		break;

	case ETurnMoveState::Active:
		MoveTurn(DeltaTime);

		Alpha = FMath::Clamp(countTime, 0.0f, 1.0f);
		Size.X = FMath::Lerp(0.0f, StartSize.X * MaxActorSize, Alpha);
		Size.Y = FMath::Lerp(0.0f, StartSize.X * MaxActorSize, Alpha);
		Size.Z = FMath::Lerp(0.0f, StartSize.X * MaxActorSize, Alpha);
		SetActorScale3D(Size);
		break;
	}
}

// 生成されたときの演出
void ATurnMoveActor::MoveRoll()
{
	float Alpha = FMath::Clamp(countTime / SpawnTime, 0.0f, 1.0f);
	FVector Size;
	Size.X = FMath::Lerp(0.0f, StartSize.X, Alpha);
	Size.Y = FMath::Lerp(0.0f, StartSize.Y, Alpha);
	Size.Z = FMath::Lerp(0.0f, StartSize.Z, Alpha);
	SetActorScale3D(Size);

	FRotator Rotator = GetActorRotation();
	Rotator.Roll += RollSpeed;

	if (countTime > SpawnTime)
	{
		Rotator.Roll = 0.0f;
		countTime = 0.0f;
		TurnMoveState = ETurnMoveState::Ready;
	}

	SetActorRotation(Rotator);
}

// 待機しているときの動き
void ATurnMoveActor::MoveDrift()
{
	if (!bIsMove)
	{
		// ランダムな目的地を作る
		DriftLocation.X = FMath::RandRange(-DriftRange, DriftRange);
		DriftLocation.Y = FMath::RandRange(-DriftRange, DriftRange);
		DriftLocation.Z = FMath::RandRange(-DriftRange, DriftRange);
		DriftLocation += BaseLocation;
		bIsMove = true;
	}

	// 加速と減速のある移動
	float Alpha = FMath::Clamp(countTime / DriftTime, 0.0f, 1.0f);
	FVector CurrentLocation = FMath::Lerp(StartLocation, DriftLocation, Alpha);

	if (Alpha >= 1.0f)
	{
		StartLocation = DriftLocation;
		countTime = 0.0f;
		bIsMove = false;
	}

	SetActorLocation(CurrentLocation);
}

void ATurnMoveActor::MoveTurn(float DeltaTime)
{
	FVector direction = (ZoneActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector forward = GetActorForwardVector().GetSafeNormal();

	// 内積を求める(成す角)
	float Dot = FVector::DotProduct(forward, direction);
	float theta = FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f));

	// 外積を求める
	FVector Cross = FVector::CrossProduct(forward, direction);

	
	FRotator Rotator = GetActorRotation();
	if (FMath::RadiansToDegrees(theta) > MaxRotation)
	{
		// クォータニオンを設定
		FQuat Quaternion(Cross.GetSafeNormal(), theta * RotationSpeed * DeltaTime);

		// 回転を更新
		AddActorWorldRotation(Quaternion);

		// Rollをなくして横転しないように
		Rotator = GetActorRotation();
		Rotator.Roll = 0.0f;

	}
	// 角度が小さいなら目的地に回転させる
	else
	{
		Rotator = direction.Rotation();
	}

	SetActorRotation(Rotator);
	AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime);
}

void ATurnMoveActor::OnOverlapBegin(
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
			if (IsValid(EnemyManager))
			{
				EnemyManager->NotifyHitPlayer(EEnemyAttackType::Normal);
			}
			

			ZoneActor->Destroy();
			Destroy();
		}

		if (OtherComp->ComponentHasTag(TEXT("Zone")))
		{
			if (ZoneActor->GetIsHitPlayer())
			{
				if (IsValid(EnemyManager))
				{
					EnemyManager->NotifyHitPlayer(EEnemyAttackType::Normal);
				}
			}
			ZoneActor->Destroy();
			Destroy();
		}
	}
}

FVector ATurnMoveActor::GetSpawnLocation()
{
	return BaseLocation;
}

void ATurnMoveActor::MoveActive()
{
	TurnMoveState = ETurnMoveState::Active;
	if (IsValid(EnemyActor))
	{
		countTime = 0.0f;

		// EnemyActorからデータの取得
		MaxActorSize = EnemyActor->GetAttackActorSize();
		MoveSpeed = EnemyActor->GetAttackMoveSpeed();
		RotationSpeed = EnemyActor->GetAttackRotationSpeed();
		ZoneActor = EnemyActor->GetZoneActor();

		// 新しく向きを設定
		FVector direction = GetActorLocation() - ZoneActor->GetActorLocation();
		FRotator Rotator = direction.Rotation();
		Rotator += EnemyActor->GetActorRotator();
		SetActorRotation(Rotator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor is not set owner in TurnMoveActor."));
		bIsValid = false;
	}
}
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
		if (ZoneActor->GetActorLocation().Z > GetActorLocation().Z)
		{
			ZoneActor->Destroy();
			Destroy();
		}

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
	FVector direction = ZoneActor->GetActorLocation() - GetActorLocation();
	FVector forward = GetActorForwardVector().GetSafeNormal();

	// 内積を求める(成す角)
	float Dot = FVector::DotProduct(forward, direction.GetSafeNormal());
	float theta = FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f));

	// 外積を求める
	FVector Cross = FVector::CrossProduct(forward, direction.GetSafeNormal());
	Cross.Normalize();

	FRotator Rotator = GetActorRotation();
	if (!Cross.IsNearlyZero() && GetActorLocation().X > ZoneActor->GetActorLocation().X)
	{
		float Alpha = FMath::Clamp(FMath::RadiansToDegrees(theta) / 180.0f, 0.0f, 1.0f);
		float Angle = FMath::Lerp(RotationSpeed, theta, Alpha);

		// クォータニオンを設定
		FQuat Quaternion(Cross, Angle * DeltaTime);

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

FVector ATurnMoveActor::CalculateQuadraticBezier(FVector P0, FVector P1, FVector P2, float T)
{
	// 2次ベジェ曲線の数式: B(t) = (1-t)^2 * P0 + 2(1-t)*t * P1 + t^2 * P2
	float OneMinusT = 1.0f - T;

	FVector Term0 = (OneMinusT * OneMinusT) * P0;
	FVector Term1 = (2.0f * OneMinusT * T) * P1;
	FVector Term2 = (T * T) * P2;

	return Term0 + Term1 + Term2;
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
		StartLocation = GetActorLocation();
		TargetDirection = GetActorLocation() - ZoneActor->GetActorLocation();
		FRotator Rotator = TargetDirection.Rotation();
		Rotator += EnemyActor->GetActorRotator();
		SetActorRotation(Rotator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor is not set owner in TurnMoveActor."));
		bIsValid = false;
	}
}
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

	bIsMoveActive = false;
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

	if (bIsMoveActive)
	{
		TurnMove(DeltaTime);

		countTime += DeltaTime;
		float Alpha = FMath::Clamp(countTime, 0.0f, 1.0f);
		float Size = FMath::Lerp(0.0f, MaxActorSize, Alpha);
		SetActorScale3D(FVector(Size, Size, Size));
	}
}

void ATurnMoveActor::TurnMove(float DeltaTime)
{
	FVector direction = (ZoneActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector forward = GetActorForwardVector().GetSafeNormal();

	// 内積を求める　(成す角)
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

void ATurnMoveActor::MoveActive()
{
	bIsMoveActive = true;
	if (IsValid(EnemyActor))
	{
		MaxActorSize = EnemyActor->GetAttackActorSize();
		MoveSpeed = EnemyActor->GetAttackMoveSpeed();
		RotationSpeed = EnemyActor->GetAttackRotationSpeed();
		ZoneActor = EnemyActor->GetZoneActor();
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
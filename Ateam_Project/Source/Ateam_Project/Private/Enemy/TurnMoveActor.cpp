// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TurnMoveActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

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
	
	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyData is not set in SubEnemy."));
		bIsConfigured = false;
	}
	else
	{
		bIsConfigured = EnemyData->IsConfigured();
	}

	if (bIsConfigured)
	{
		SetPlayerPawn();
	}
}

void ATurnMoveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(BoxComp) || !bIsConfigured) return;

	Move(DeltaTime);

}

void ATurnMoveActor::SetPlayerPawn()
{
	PlayerPawn = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyData->PlayerPawn);
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is not set in FallActor"));
		bIsConfigured = false;
	}
}

void ATurnMoveActor::Move(float DeltaTime)
{
	FVector direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector forward = GetActorForwardVector().GetSafeNormal();

	// 内積を求める　(成す角)
	float Dot = FVector::DotProduct(forward, direction);
	float theta = FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f));

	// 外積を求める
	FVector Cross = FVector::CrossProduct(forward, direction);

	if (!FMath::IsNearlyZero(Cross.Z))
	{
		// クォータニオンを設定
		FQuat Quaternion(Cross.GetSafeNormal(), theta * RotationSpeed * DeltaTime);

		// 回転を更新
		AddActorWorldRotation(Quaternion);

		// Rollをなくして横転しないように
		FRotator Rotator = GetActorRotation();
		Rotator.Roll = 0.0f;

		SetActorRotation(Rotator);
	}
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
		UE_LOG(LogTemp, Warning, TEXT("Hit in TurnMoveActor!"));
	}
}
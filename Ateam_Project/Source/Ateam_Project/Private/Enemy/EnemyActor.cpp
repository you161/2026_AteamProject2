// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Enemy/FallActor.h"
#include "Enemy/EnemyData.h"
#include "Component/DamageFlashComponent.h"
#include "Sound/SEManager.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// アタッチされているか
	SetObjects();

	// 部位ごとの当たり判定を設定
	SetBoxCollision();

	// 移動するごとに初期化するもの
	ResetAttack();

	//点滅させる処理を取得
	DamageFlashComponent = FindComponentByClass<UDamageFlashComponent>();

	if (IsValid(HealthManager))
	{
		//ダメージ通知を受け取るよう登録する
		HealthManager->OnDamaged.AddUObject(this, &AEnemyActor::OnDamaged);
	}

}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// アタッチがされていなければ実行しない
	if (bIsValid == false) return;

	if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
	{
		if (PC->WasInputKeyJustPressed(EKeys::P))
		{
			bIsMove = true;

			for (ATurnMoveActor* Actor : TurnMoveArray)
			{
				if (IsValid(Actor))
				{
					Actor->Destroy();
				}
			}

			TurnMoveArray.Reset();

			if (GroundActor)
			{
				CenterLocation = GroundActor->GetActorLocation();
				CenterLocation.Z = GetActorLocation().Z;

				TargetLocation.X = GetActorLocation().Y;
				TargetLocation.Y = GetActorLocation().X;
				TargetLocation.Z = GetActorLocation().Z;

				Radius = FVector::Dist2D(CenterLocation, GetActorLocation());

				FVector CurrentLocation = GetActorLocation();
				CurrentAngle = atan2(CurrentLocation.Y - CenterLocation.Y, CurrentLocation.X - CenterLocation.X);
			}
		}
	}

	if (bIsMove)
	{
		FVector CurrentLocation = GetActorLocation();

		float DistanceToTarget = FVector::Dist2D(CurrentLocation, TargetLocation);

		if (DistanceToTarget <= 25.0f)
		{
			bIsMove = false;
			FVector Forward = CenterLocation - TargetLocation;
			FRotator Rotator = Forward.GetSafeNormal().Rotation();
			SetActorRotation(Rotator);
			ResetAttack();
		}
		else
		{
			// 角度を時間経過とともに進める
			CurrentAngle += AngleSpeed * DeltaTime;

			// 新しい位置を計算
			FVector NewLocation = GetActorLocation();
			NewLocation.X = CenterLocation.X + Radius * cos(CurrentAngle);
			NewLocation.Y = CenterLocation.Y + Radius * sin(CurrentAngle);

			SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

			float FutureAngle = CurrentAngle + 0.01f;
			FVector FutureLocation;
			FutureLocation.X = CenterLocation.X + Radius * cos(FutureAngle);
			FutureLocation.Y = CenterLocation.Y + Radius * sin(FutureAngle);
			FutureLocation.Z = NewLocation.Z;

			// 現在地から未来の位置へ向かう回転を計算して適用
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, FutureLocation);
			SetActorRotation(TargetRotation);
		}
	}

	// 攻撃が生成されるまでの時間
	if (!bIsReady && !bIsMove)
	{
		ReadyTime += DeltaTime;
		if (ReadyTime > EnemyData->SpawnTime)
		{
			bIsReady = true;
		}
	}
	// 攻撃が生成されたら
	else if (bIsReady && !bIsMove)
	{
		// 攻撃を設定
		SetAttackActor();

		TotalTime += DeltaTime;

		// 攻撃を呼び出す
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
	bIsMove = false;
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

void AEnemyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//登録を解除する
	if (IsValid(HealthManager))
	{
		HealthManager->OnDamaged.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

//ダメージを受けた時の処理
void AEnemyActor::OnDamaged(ECharacterType DamagedType)
{
	//自分宛てでなければ何もしない
	if (DamagedType != ECharacterType::Enemy)
	{
		return;
	}

	if (IsValid(DamageFlashComponent))
	{
		DamageFlashComponent->PlayFlash();
	}

	if (IsValid(SEManager))
	{
		SEManager->PlaySE(ESEType::EnemyDamage);
	}
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
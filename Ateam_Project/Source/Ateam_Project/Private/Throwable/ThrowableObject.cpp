#include "Throwable/ThrowableObject.h"
#include "Components/SphereComponent.h"
#include "Throwable/ThrowableObjectData.h"
#include "Throwable/ThrowableManager.h"
#include "Player/PlayerPawn.h"
#include "Enemy/EnemyActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
	//当たり判定の初期半径
	constexpr float DefaultCollisionRadius = 150.0f;
	//投擲オブジェクトが使うコリジョンプロファイル名
	const FName ThrowableCollisionProfile(TEXT("OverlapAllDynamic"));
}

AThrowableObject::AThrowableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//SphereCollisionを生成してルートにする
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->InitSphereRadius(DefaultCollisionRadius);
	SphereCollision->SetCollisionProfileName(ThrowableCollisionProfile);

	//メッシュを生成してルートにアタッチする
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereCollision);
	//当たり判定はSphereCollisionが担当するのでメッシュ側は無効にする
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AThrowableObject::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AThrowableObject::OnOverlap);

	CurrentState = EThrowableState::Pickable;
	BaseScale = GetActorScale3D();
}

void AThrowableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateThrownMovement(DeltaTime);
	UpdateHeldRotation(DeltaTime);
}

//生成時にマネージャーを注入する
void AThrowableObject::SetThrowableManager(AThrowableManager* InManager)
{
	ThrowableManager = InManager;
}

//持たれた時の処理
void AThrowableObject::OnPickedUp(AActor* Holder)
{
	if (CurrentState != EThrowableState::Pickable || !IsValid(Holder))
	{
		return;
	}

	CurrentState = EThrowableState::Held;
	HolderPawn = Cast<APlayerPawn>(Holder);
	SetActorTickEnabled(true);

	//プレイヤーにアタッチ
	AttachToActor(Holder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SetActorRelativeRotation(FRotator::ZeroRotator);
	UpdateHeldLocation();
}

//回収数に合わせて持っている間のスケールを更新する
void AThrowableObject::UpdateHeldScale(int32 CollectedCount)
{
	if (CurrentState != EThrowableState::Held)
	{
		return;
	}

	if (!IsValid(ThrowableData))
	{
		UE_LOG(LogThrowable, Error, TEXT("ThrowableData is null in AThrowableObject::UpdateHeldScale"));
		return;
	}

	//サイズを大きくする
	const int32 ExtraCount = FMath::Max(CollectedCount - 1, 0);
	const float Multiplier = FMath::Min(1.0f + ExtraCount * ThrowableData->ScalePerItem, ThrowableData->MaxScale);

	SetActorScale3D(BaseScale * Multiplier);
	UpdateHeldLocation();
}

//投げられた時の処理
void AThrowableObject::OnThrown(const FVector& Direction, int32 StackCount)
{
	if (CurrentState != EThrowableState::Held)
	{
		return;
	}

	//アタッチを外す
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	CurrentState = EThrowableState::Thrown;
	HolderPawn.Reset();

	CurrentDirection = Direction.GetSafeNormal();
	ThrowStackCount = StackCount;
	ThrowStartLocation = GetActorLocation();

	//投げた時点ですでに敵と重なっていたらヒット扱いにする
	TArray<AActor*> OverlappingActors;
	SphereCollision->GetOverlappingActors(OverlappingActors, AEnemyActor::StaticClass());

	if (!OverlappingActors.IsEmpty())
	{
		HitEnemy();
	}
}

//持たれている間の回転処理
void AThrowableObject::UpdateHeldRotation(float DeltaTime)
{
	if (CurrentState != EThrowableState::Held || !HolderPawn.IsValid() || !IsValid(ThrowableData))
	{
		return;
	}

	//プレイヤーが移動中のときだけ回転する
	if (!HolderPawn->GetIsMoving())
	{
		return;
	}

	//回転させる
	AddActorLocalRotation(ThrowableData->RotateSpeed * DeltaTime);
}

//アイテムの大きさに合わせて持つ位置を更新する
void AThrowableObject::UpdateHeldLocation()
{
	if (!HolderPawn.IsValid() || !IsValid(ThrowableData))
	{
		return;
	}

	//プレイヤーのカプセルサイズを取得
	float PlayerRadius = 0.0f;
	float PlayerHalfHeight = 0.0f;
	HolderPawn->GetSimpleCollisionCylinder(PlayerRadius, PlayerHalfHeight);

	//オブジェクトの半径
	const float ItemRadius = SphereCollision->GetScaledSphereRadius();

	//埋まらないように前方向と下方向のオフセットを調整する
	FVector NewOffset = ThrowableData->Offset;
	NewOffset.X = FMath::Max(ThrowableData->Offset.X, PlayerRadius + ItemRadius);
	NewOffset.Z = FMath::Max(ThrowableData->Offset.Z, -PlayerHalfHeight + ItemRadius);

	SetActorRelativeLocation(NewOffset);
}

//回収数に合わせて持っている間の色を更新する
void AThrowableObject::UpdateHeldColor(int32 CollectedCount)
{
	if (CurrentState != EThrowableState::Held)
	{
		UE_LOG(LogThrowable, Warning, TEXT("[%s] UpdateHeldColor skipped: state is not Held"), *GetName());
		return;
	}

	if (!IsValid(ThrowableData))
	{
		UE_LOG(LogThrowable, Error, TEXT("ThrowableData is null in AThrowableObject::UpdateHeldColor"));
		return;
	}

	if (ThrowableData->StackColors.IsEmpty())
	{
		UE_LOG(LogThrowable, Warning, TEXT("[%s] UpdateHeldColor skipped: StackColors is empty"), *GetName());
		return;
	}

	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogThrowable, Error, TEXT("MeshComponent is null in AThrowableObject::UpdateHeldColor"));
		return;
	}

	//個数に対応する色を選ぶ
	const int32 ColorIndex = FMath::Clamp(CollectedCount - 1, 0, ThrowableData->StackColors.Num() - 1);
	const FLinearColor& NewColor = ThrowableData->StackColors[ColorIndex];

	const int32 MaterialCount = MeshComponent->GetNumMaterials();
	
	for (int32 i = 0; i < MaterialCount; ++i)
	{
		//すでに動的マテリアルならそれを使い、なければ作成する
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(i));
		if (!IsValid(DynamicMaterial))
		{
			DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(i);
		}

		if (!IsValid(DynamicMaterial))
		{
			UE_LOG(LogThrowable, Error, TEXT("[%s] Failed to create dynamic material at slot %d"), *GetName(), i);
			continue;
		}

		DynamicMaterial->SetVectorParameterValue(ThrowableData->ColorParameterName, NewColor);
	}
}

//投げられて指定方向に飛んでいく処理
void AThrowableObject::UpdateThrownMovement(float DeltaTime)
{
	if (CurrentState != EThrowableState::Thrown)
	{
		return;
	}

	if (!IsValid(ThrowableData))
	{
		UE_LOG(LogThrowable, Error, TEXT("ThrowableData is null in AThrowableObject::UpdateThrownMovement"));
		return;
	}

	//移動させる
	const FVector Delta = CurrentDirection * ThrowableData->MoveSpeed * DeltaTime;
	AddActorWorldOffset(Delta, false);

	//飛距離がMaxThrowDistanceを超えたら消す
	if (FVector::Dist(ThrowStartLocation, GetActorLocation()) >= ThrowableData->MaxThrowDistance)
	{
		Destroy();
	}
}

//敵に当たった時の処理
void AThrowableObject::HitEnemy()
{
	if (IsValid(ThrowableManager))
	{
		ThrowableManager->NotifyHitEnemy(ThrowStackCount);
	}

	//同じフレームに複数回当たらないようにする
	SetActorEnableCollision(false);
	Destroy();
}

//他のアクタと重なった時に呼ばれる
void AThrowableObject::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	//プレイヤーもしくは持たれているオブジェクトと重なったら拾われる
	if (CurrentState == EThrowableState::Pickable)
	{
		const AThrowableObject* OtherItem = Cast<AThrowableObject>(OtherActor);
		const bool bIsPickerOverlapped = OtherActor->IsA<APlayerPawn>() || (IsValid(OtherItem) && OtherItem->IsHeld());

		if (!bIsPickerOverlapped)
		{
			return;
		}

		if (!IsValid(ThrowableManager))
		{
			UE_LOG(LogThrowable, Error, TEXT("ThrowableManager is null in AThrowableObject::OnOverlap"));
			return;
		}

		ThrowableManager->PickUp(this);
		return;
	}

	//投げられている間に敵と重なったら当たったことを通知して消える
	if (CurrentState == EThrowableState::Thrown && OtherActor->IsA<AEnemyActor>())
	{
		HitEnemy();
		return;
	}
}
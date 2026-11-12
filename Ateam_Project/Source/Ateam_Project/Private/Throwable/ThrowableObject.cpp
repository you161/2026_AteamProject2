
#include "Throwable/ThrowableObject.h"
#include "Components/SphereComponent.h"
#include "Throwable/ThrowableObjectData.h"
#include "Throwable/ThrowableManager.h"
#include "Player/PlayerPawn.h"
#include "Enemy/EnemyActor.h"
#include "Health/HealthManager.h"
#include "Player/PlayerLevelManager.h"

AThrowableObject::AThrowableObject()
{
	PrimaryActorTick.bCanEverTick = true;

    //SphereCollisionを生成してルートにする
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SetRootComponent(SphereCollision);
    SphereCollision->InitSphereRadius(150.0f);
    SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AThrowableObject::BeginPlay()
{
	Super::BeginPlay();

    //SphereCollisionの中にデータがあれば{}内を実行する
    if (IsValid(SphereCollision))
    {
        //オーバーラップ開始時にOnOverlap関数を呼ぶように設定
        SphereCollision->OnComponentBeginOverlap.AddDynamic(
            this,
            &AThrowableObject::OnOverlap
        );
    }

    CurrentState = EThrowableState::Pickable;
    BaseScale = GetActorScale3D();
}

void AThrowableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateThrownMovement(DeltaTime);
    UpdateHeldRotation(DeltaTime);
}

//持たれているときの処理
void AThrowableObject::OnPickedUp(AActor* Holder)
{
    if (!IsValid(Holder))
    {
        return;
    }

	if (!IsValid(ThrowableManager))
	{
		UE_LOG(LogTemp, Error, TEXT("ThrowableManager is null in AThrowableObject::OnPickedUp"));
		return;
	}

	if (!IsValid(ThrowableData))
	{
		UE_LOG(LogTemp, Error, TEXT("ThrowableData is null in AThrowableObject::OnPickedUp"));
		return;
	}

	if (CurrentState == EThrowableState::Held)
	{
		//スケールを調整する
        const float Multiplier = FMath::Min(1.0f + ThrowableManager->GetCollectedCount() * ThrowableData->ScalePerItem, ThrowableData->MaxScale);
        SetActorScale3D(BaseScale * Multiplier);
        UpdateHeldLocation();
		return;
	}

    CurrentState = EThrowableState::Held;

    //プレイヤーにアタッチ
    AttachToActor(Holder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    SetActorRelativeRotation(FRotator::ZeroRotator);
    UpdateHeldLocation();
}

//投げられた時の処理
void AThrowableObject::OnThrown(const FVector& Direction)
{
    //アタッチを外す(今のワールド位置を維持)
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    CurrentState = EThrowableState::Thrown;
    SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SetActorEnableCollision(true);

    CurrentDirection = Direction;
    ThrowStartLocation = GetActorLocation();
}

//持たれている間の回転処理
void AThrowableObject::UpdateHeldRotation(float DeltaTime)
{
    if (CurrentState != EThrowableState::Held || !IsValid(PlayerPawn))
    {
        return;
    }

    //プレイヤーが移動中のときだけ回転する
    if (!PlayerPawn->GetIsMoving())
    {
        return;
    }

    //プレイヤー基準のローカル回転として加算する
    AddActorLocalRotation(ThrowableData->RotateSpeed * DeltaTime);
}

void AThrowableObject::UpdateHeldLocation()
{
    if (!IsValid(PlayerPawn) || !IsValid(SphereCollision))
    {
        return;
    }

    //プレイヤーのカプセルサイズを取得
    float PlayerRadius = 0.0f;
    float PlayerHalfHeight = 0.0f;
    PlayerPawn->GetSimpleCollisionCylinder(PlayerRadius, PlayerHalfHeight);

    //オブジェクトの半径
    const float ItemRadius = SphereCollision->GetScaledSphereRadius();

    FVector NewOffset = ThrowableData->Offset;

	//オブジェクトが埋まらないように前方向と下方向のオフセットを調整する
    NewOffset.X = FMath::Max(ThrowableData->Offset.X, PlayerRadius + ItemRadius);
    NewOffset.Z = FMath::Max(ThrowableData->Offset.Z, -PlayerHalfHeight + ItemRadius);

    SetActorRelativeLocation(NewOffset);
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
		UE_LOG(LogTemp, Error, TEXT("ThrowableData is null in AThrowableObject::UpdateThrownMovement"));
		return;
	}

	//指定方向に移動する
    const FVector Delta = CurrentDirection * ThrowableData->MoveSpeed * DeltaTime;
    AddActorWorldOffset(Delta, true);

	//飛距離がMaxThrowDistanceを超えたら消す
    if (FVector::Dist(ThrowStartLocation, GetActorLocation()) >= ThrowableData->MaxThrowDistance)
    {
        Destroy();
    }
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
	//プレイヤーもしくは持たれているオブジェクトと重なった場合に拾う処理を行う
    if (CurrentState == EThrowableState::Pickable && !bIsCollected && IsValid(ThrowableManager))
    {
		const APlayerPawn* OtherPlayer = Cast<APlayerPawn>(OtherActor);
        const AThrowableObject* OtherItem = Cast<AThrowableObject>(OtherActor);

        if (OtherPlayer || (OtherItem && OtherItem->IsHeld()))
        {
            if (ThrowableManager->PickUp(this))
            {
                bIsCollected = true;
            }
            return;
        }
    }

	//投げられたオブジェクトが敵と重なった場合にダメージを与える処理を行う
    if (CurrentState == EThrowableState::Thrown && Cast<AEnemyActor>(OtherActor) 
        && IsValid(HealthManager) && IsValid(PlayerLevelManager))
    {
        HealthManager->TakeDamage(CharactorType::Enemy);
        PlayerLevelManager->AddExp();
        Destroy();
        return;
    }
}

#include "Throwable/ThrowableObject.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerPawn.h"
#include "Throwable/ThrowableManager.h"

AThrowableObject::AThrowableObject()
{
	PrimaryActorTick.bCanEverTick = true;

    //SphereCollisionを生成してルートにする
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SetRootComponent(SphereCollision);
    SphereCollision->InitSphereRadius(150.0f);
    SphereCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
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

		//オーバーラップ終了時にOnEndOverlap関数を呼ぶように設定
		SphereCollision->OnComponentEndOverlap.AddDynamic(
			this,
			&AThrowableObject::OnEndOverlap
		);

        //SphereCollisionが見つかった場合の確認ログ
        UE_LOG(LogTemp, Warning, TEXT("SphereCollision found and saved to variable: %s"), *GetName());
    }

    CurrentState = EThrowableState::Pickable;
}

void AThrowableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateThrownMovement(DeltaTime);
}

//持たれているときの処理
void AThrowableObject::OnPickedUp(AActor* Holder)
{
    if (!IsValid(Holder))
    {
        return;
    }

    CurrentState = EThrowableState::Held;

    //持たれている間は当たり判定を切る(プレイヤーを押したり、再度Overlapしたりしないように)
    SetActorEnableCollision(false);

    //プレイヤーにアタッチ
    AttachToActor(Holder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    //プレイヤーから見た相対位置(前方100、高さ50)
    SetActorRelativeLocation(FVector(100.0f, 0.0f, 50.0f));
    SetActorRelativeRotation(FRotator::ZeroRotator);
}

//投げられた時の処理
void AThrowableObject::OnThrown(const FVector& Direction)
{
    //アタッチを外す(今のワールド位置を維持)
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    CurrentState = EThrowableState::Thrown;
    SetActorEnableCollision(true);

    CurrentDirection = Direction;
}

//投げられて指定方向に飛んでいく処理
void AThrowableObject::UpdateThrownMovement(float DeltaTime)
{
    if (CurrentState != EThrowableState::Thrown)
    {
        return;
    }

    const FVector Delta = CurrentDirection * 500.0f * DeltaTime;
    AddActorWorldOffset(Delta, true);
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
    //プレイヤー以外は無視
    if (Cast<APlayerPawn>(OtherActor))
    {
        //拾える状態のときだけ候補に登録する
        if (CurrentState == EThrowableState::Pickable && IsValid(ThrowableManager))
        {
            ThrowableManager->AddCandidate(this);
        }
    }
}

void AThrowableObject::OnEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	//触れたActorがプレイヤーかチェック
	if (Cast<APlayerPawn>(OtherActor))
	{
        if (IsValid(ThrowableManager))
        {
            ThrowableManager->RemoveCandidate(this);
        }
	}
}
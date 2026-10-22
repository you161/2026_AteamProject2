
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
}

// 他のアクタと重なった時に呼ばれる
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
    if (!Cast<APlayerPawn>(OtherActor))
    {
        return;
    }

    //拾える状態のときだけ候補に登録する
    if (CurrentState == EThrowableState::Pickable && IsValid(ThrowableManager))
    {
        ThrowableManager->AddCandidate(this);
    }
}

void AThrowableObject::OnEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	// 触れたActorがプレイヤーかチェック
	if (Cast<APlayerPawn>(OtherActor))
	{
        if (IsValid(ThrowableManager))
        {
            ThrowableManager->RemoveCandidate(this);
        }
	}
}
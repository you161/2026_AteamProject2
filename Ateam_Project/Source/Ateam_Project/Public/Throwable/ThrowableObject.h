
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableState.h"
#include "ThrowableObject.generated.h"

class USphereComponent;
class AThrowableManager;

/**
 *投げることができるアイテム
 */
UCLASS()
class ATEAM_PROJECT_API AThrowableObject : public AActor
{
	GENERATED_BODY()
	
public:	

	AThrowableObject();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnPickedUp(AActor* Holder);
	void OnThrown(const FVector& Direction);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	//他のActorと重なった時に呼ばれる関数
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	//他のActorとの重なりが終わった時に呼ばれる関数
	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ThrowableManager")
	TObjectPtr<AThrowableManager> ThrowableManager = nullptr;

private:

	EThrowableState CurrentState = EThrowableState::Pickable;
};
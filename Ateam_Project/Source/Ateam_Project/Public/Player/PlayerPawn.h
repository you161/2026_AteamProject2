
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UInputManager;
class UPlayerData;
class AActor;

/**
 * 入力の値を受け取り、プレイヤーの移動やジャンプの状態を管理するクラス
 */
UCLASS()
class ATEAM_PROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	void InitializeInputManager();

private:
	void Move(float DeltaTime);
	void Rotate(float DeltaTime);

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UPlayerData> PlayerData = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<AActor> Camera = nullptr;

private:
	UPROPERTY()
	UInputManager* InputManager = nullptr;

	FVector3d CurrentDirection = FVector3d::ZeroVector;
};
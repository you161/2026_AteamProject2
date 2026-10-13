#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UInputManager;
class UPlayerData;

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

	//コントローラーがPossessしたときに呼ばれる
	virtual void PossessedBy(AController* NewController) override;

	//コントローラーがUnPossessしたときに呼ばれる
	virtual void UnPossessed() override;

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
	UPROPERTY(Transient)
	TObjectPtr<UInputManager> InputManager = nullptr;

	FVector CurrentDirection = FVector::ZeroVector;
};
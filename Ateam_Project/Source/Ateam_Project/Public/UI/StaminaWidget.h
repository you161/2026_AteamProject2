
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaWidget.generated.h"

class UProgressBar;
class AStaminaManager;

UCLASS()
class UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Managerを設定して、通知を受け取れるようにする
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetStaminaManager(AStaminaManager* InManager);

protected:
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void HandleStaminaChanged(float Ratio, bool bCanSprint);

	//スタミナ表示UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar = nullptr;
	//通常色
	UPROPERTY(EditAnywhere, Category = "Stamina")
	FLinearColor NormalColor = FLinearColor::Yellow;
	//スタミナが切れた時の色
	UPROPERTY(EditAnywhere, Category = "Stamina")
	FLinearColor ExhaustedColor = FLinearColor::Red;

	TWeakObjectPtr<AStaminaManager> StaminaManager = nullptr;
};
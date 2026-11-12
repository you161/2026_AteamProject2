
#include "UI/StaminaWidget.h"
#include "Player/StaminaManager.h"
#include "Components/ProgressBar.h"

void UStaminaWidget::SetStaminaManager(AStaminaManager* InManager)
{
	if (!IsValid(InManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("[StaminaWidget] StaminaManager is invalid."));
		return;
	}

	//古いManagerの購読を解除してから、新しいものを登録する
	if (StaminaManager.IsValid())
	{
		StaminaManager->OnStaminaChanged.RemoveDynamic(this, &UStaminaWidget::HandleStaminaChanged);
	}

	StaminaManager = InManager;
	StaminaManager->OnStaminaChanged.AddDynamic(this, &UStaminaWidget::HandleStaminaChanged);

	//登録した時点の値で、すぐ表示を合わせる
	HandleStaminaChanged(StaminaManager->GetStaminaRatio(), StaminaManager->GetCanSprint());
}

void UStaminaWidget::NativeDestruct()
{
	if (StaminaManager.IsValid())
	{
		StaminaManager->OnStaminaChanged.RemoveDynamic(this, &UStaminaWidget::HandleStaminaChanged);
	}

	Super::NativeDestruct();
}

void UStaminaWidget::HandleStaminaChanged(float Ratio, bool bCanSprint)
{
	if (!IsValid(StaminaBar))
	{
		return;
	}

	StaminaBar->SetPercent(Ratio);
	//枯渇してダッシュできない間は色を変える
	StaminaBar->SetFillColorAndOpacity(bCanSprint ? NormalColor : ExhaustedColor);
}
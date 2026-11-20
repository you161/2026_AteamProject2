#include "UI/StaminaWidget.h"
#include "Player/StaminaManager.h"
#include "Components/Image.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//左端を基準に縮むようにピボットを左中央に設定する
	if (IsValid(StaminaImage))
	{
		StaminaImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));
	}
}

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
	if (!IsValid(StaminaImage))
	{
		return;
	}

	//横スケールで残量を表現する
	StaminaImage->SetRenderScale(FVector2D(FMath::Clamp(Ratio, 0.0f, 1.0f), 1.0f));
	//枯渇してダッシュできない間は色を変える
	StaminaImage->SetColorAndOpacity(bCanSprint ? NormalColor : ExhaustedColor);
}
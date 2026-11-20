// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/UIManager.h"
#include "Health/HitPointBar.h"
#include "Health/HealthManager.h"
#include "UI/StaminaWidget.h"
#include "Player/StaminaManager.h"

AUIManager::AUIManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUIManager::BeginPlay()
{
	Super::BeginPlay();

	UHitPointBar* PlayerBar = nullptr;
	UHitPointBar* EnemyBar = nullptr;
	UStaminaWidget* StaminaWidget = nullptr;

	for (FWidgetEntry Entry : WidgetEntry)
	{
		if (!IsValid(Entry.WidgetClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetEntry is not set WidgetClass in UIManager"));
			return;
		}

		UUserWidget* Created = CreateWidget<UUserWidget>(GetWorld(), Entry.WidgetClass);
		Created->AddToViewport();
		SpawnedWidgets.Add(Created);

		switch (Entry.WidgetType)
		{
		case EWidgetType::UserWidget:
			break;
			
		case EWidgetType::PlayerHBar:
			PlayerBar = Cast<UHitPointBar>(Created);
			break;

		case EWidgetType::EnemyHPBar:
			EnemyBar = Cast<UHitPointBar>(Created);
			break;

		case EWidgetType::Stamina:
			StaminaWidget = Cast<UStaminaWidget>(Created);
			break;
		}
	}

	if(IsValid(HealthManager) && IsValid(PlayerBar) && IsValid(EnemyBar))
	{
		HealthManager->InitializeHealth(PlayerBar, EnemyBar);
	}

	if (IsValid(StaminaManager) && IsValid(StaminaWidget))
	{
		StaminaWidget->SetStaminaManager(StaminaManager);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StaminaWidget or StaminaManager is not valid in UIManager."));

	}
}

void AUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
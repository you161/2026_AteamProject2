// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/ChangeLevel.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Level/LevelManager.h"

AChangeLevel::AChangeLevel() {}

void AChangeLevel::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LevelManagerClass))
	{
		LevelManager = Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelManager::StaticClass()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManagerClass is not set in ChangeLevel"));
		return;
	}

	// Enhanced Input Systemの設定
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is not set in ChangeLevel"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!IsValid(InputSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("InputSubsystem is not set in ChangeLevel"));
		return;
	}

	if (InputMappingContext == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InputMappingContext is not set in ChangeLevel."));
		return;
	}

	InputSubsystem->AddMappingContext(InputMappingContext, 0);
}

void AChangeLevel::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input Systemの設定
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent is not EnhancedInputComponent."));
		return;
	}

	if (IsValid(ChangeLevelAction))
	{
		EnhancedInputComponent->BindAction(
			ChangeLevelAction,
			ETriggerEvent::Triggered,
			this,
			&AChangeLevel::ChangeLevel
		);

		EnhancedInputComponent->BindAction(
			ChangeLevelAction,
			ETriggerEvent::Completed,
			this,
			&AChangeLevel::ChangeLevel
		);

		EnhancedInputComponent->BindAction(
			ChangeLevelAction,
			ETriggerEvent::Canceled,
			this,
			&AChangeLevel::ChangeLevel
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChangeLevelAction is not set in ChangeLevel"));
	}
}

void AChangeLevel::ChangeLevel()
{
	if (!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager is not set in ChangeLevel"));
		return;
	}

	switch (LevelName) {
	case LevelSeed::None:
		UE_LOG(LogTemp, Error, TEXT("LevelName is not set in ChangeLevel"));
		break;
	case LevelSeed::TitleLevel:
		LevelManager->LoadMovieLevel();
		break;
	case LevelSeed::MovieLevel:
		LevelManager->LoadMainLevel();
		break;
	case LevelSeed::MainLevel:
		break;
	case LevelSeed::ClearLevel:
		LevelManager->LoadTitleLevel();
		break;
	case LevelSeed::GameOverLevel:
		LevelManager->LoadTitleLevel();
		break;
	}
}
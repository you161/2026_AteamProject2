
#include "Input/InputReceiver.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/InputManager.h"

AInputReceiver::AInputReceiver(){}

void AInputReceiver::BeginPlay()
{
	Super::BeginPlay();

	InputManager = NewObject<UInputManager>(this);

	if (InputManager == nullptr)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (LocalPlayer == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (InputSubsystem == nullptr)
	{
		return;
	}

	if (InputMappingContext == nullptr)
	{
		return;
	}

	InputSubsystem->AddMappingContext(InputMappingContext, 0);
}

void AInputReceiver::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	if (MoveAction != nullptr)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AInputReceiver::Move
		);
	}

	if (JumpAction != nullptr)
	{
		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&AInputReceiver::Jump
		);
	}
}

void AInputReceiver::Move(const FInputActionValue& Value)
{
	if (InputManager == nullptr)
	{
		return;
	}

	const FVector2D MoveInput = Value.Get<FVector2D>();

	InputManager->SetMoveInput(MoveInput);
}

void AInputReceiver::Jump(const FInputActionValue& Value)
{
	if (InputManager == nullptr)
	{
		return;
	}

	InputManager->SetJumpPressed(true);
}
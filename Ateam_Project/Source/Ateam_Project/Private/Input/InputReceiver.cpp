
#include "Input/InputReceiver.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/InputManager.h"

AInputReceiver::AInputReceiver(){}

void AInputReceiver::BeginPlay()
{
	Super::BeginPlay();

	GetInputManager();

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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent is not EnhancedInputComponent."));
		return;
	}

	if (MoveAction == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MoveAction is not set. Use a Blueprint subclass of InputReceiver."));
	}

	if (MoveAction != nullptr)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AInputReceiver::Move
		);

		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Completed,
			this,
			&AInputReceiver::Move
		);

		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Canceled,
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

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&AInputReceiver::JumpReleased
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

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("InputReceiver Move: X=%f, Y=%f"),
		MoveInput.X,
		MoveInput.Y
	);

	InputManager->SetMoveInput(MoveInput);
}

void AInputReceiver::Jump()
{
	if (InputManager == nullptr)
	{
		return;
	}

	InputManager->SetJumpPressed(true);
}

void AInputReceiver::JumpReleased()
{
	if (InputManager == nullptr)
	{
		return;
	}

	InputManager->SetJumpPressed(false);
}

UInputManager* AInputReceiver::GetInputManager()
{
	if (InputManager == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("InputManager is null in GetInputManager."));

		InputManager = NewObject<UInputManager>(this);
	}

	return InputManager;
}
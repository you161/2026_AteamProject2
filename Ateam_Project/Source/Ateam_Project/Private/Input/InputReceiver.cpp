#include "Input/InputReceiver.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/InputManager.h"

AInputReceiver::AInputReceiver()
{
	//コントローラーが存在する時点でInputManagerも必ず存在するようにする
	InputManager = CreateDefaultSubobject<UInputManager>(TEXT("InputManager"));
}

void AInputReceiver::BeginPlay()
{
	Super::BeginPlay();

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
		UE_LOG(LogTemp, Error, TEXT("InputMappingContext is not set in InputReceiver."));
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
		UE_LOG(LogTemp, Error, TEXT("MoveAction is not set in InputReceiver."));
	}
	else
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

	InputManager->SetMoveInput(Value.Get<FVector2D>());
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
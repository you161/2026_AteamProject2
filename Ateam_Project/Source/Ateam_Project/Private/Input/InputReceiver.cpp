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

	//Enhanced Input Systemの設定
	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (!IsValid(LocalPlayer))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!IsValid(InputSubsystem))
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

	//Enhanced Input Systemの設定
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent is not EnhancedInputComponent."));
		return;
	}

	if (IsValid(MoveAction))
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MoveAction is not set in InputReceiver."));
	}

	if (IsValid(JumpAction))
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JumpAction is not set in InputReceiver."));
	}

	if (IsValid(LookAction))
	{
		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AInputReceiver::Look
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Completed,
			this,
			&AInputReceiver::Look
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Canceled,
			this,
			&AInputReceiver::Look
		);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("LookAction is not set in InputReceiver."));
	}
}

//入力をInputManagerに渡す関数群
void AInputReceiver::Move(const FInputActionValue& Value)
{
	if (!IsValid(InputManager))
	{
		UE_LOG(LogTemp, Error, TEXT("InputManager is not valid in Move function."));
		return;
	}

	InputManager->SetMoveInput(Value.Get<FVector2D>());
}

//ジャンプ入力をInputManagerに渡す関数群
void AInputReceiver::Jump()
{
	if (!IsValid(InputManager))
	{
		UE_LOG(LogTemp, Error, TEXT("InputManager is not valid in Jump function."));
		return;
	}

	InputManager->SetJumpPressed(true);
}

//ジャンプ入力をInputManagerに渡す関数群
void AInputReceiver::JumpReleased()
{
	if (!IsValid(InputManager))
	{
		UE_LOG(LogTemp, Error, TEXT("InputManager is not valid in JumpReleased function."));
		return;
	}

	InputManager->SetJumpPressed(false);
}

//マウス入力をInputManagerに渡す関数群
void AInputReceiver::Look(const FInputActionValue& Value)
{
	if (!IsValid(InputManager))
	{
		UE_LOG(LogTemp, Error, TEXT("InputManager is not valid in Look function."));
		return;
	}

	InputManager->SetMouseInput(Value.Get<FVector2D>());
}

UInputManager* AInputReceiver::GetInputManager()
{
	//InputManagerが無効な場合は新しいインスタンスを作成する
	if (!IsValid(InputManager))
	{
		InputManager = NewObject<UInputManager>(this);
		UE_LOG(LogTemp, Warning, TEXT("InputManager was invalid. A new instance has been created."));
	}

	return InputManager;
}
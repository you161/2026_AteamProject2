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

	//EnhancedInputSystemの設定
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent is not EnhancedInputComponent."));
		return;
	}

	if (!IsValid(InputManager))
	{
		UE_LOG(LogTemp, Error, TEXT("InputManager is not valid in SetupInputComponent."));
		return;
	}

	//移動アクション
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

	//カメラマウス操作アクション
	if (IsValid(LookMouseAction))
	{
		EnhancedInputComponent->BindAction(
			LookMouseAction,
			ETriggerEvent::Triggered,
			this,
			&AInputReceiver::LookMouse
		);

		EnhancedInputComponent->BindAction(
			LookMouseAction,
			ETriggerEvent::Completed,
			this,
			&AInputReceiver::LookMouse
		);

		EnhancedInputComponent->BindAction(
			LookMouseAction,
			ETriggerEvent::Canceled,
			this,
			&AInputReceiver::LookMouse
		);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("LookAction is not set in InputReceiver."));
	}

	//カメラコントローラーアクション
	if (IsValid(LookGamepadAction))
	{
		EnhancedInputComponent->BindAction(
			LookGamepadAction,
			ETriggerEvent::Triggered,
			this,
			&AInputReceiver::LookGamepad
		);
		EnhancedInputComponent->BindAction(
			LookGamepadAction,
			ETriggerEvent::Completed,
			this,
			&AInputReceiver::LookGamepad
		);
		EnhancedInputComponent->BindAction(
			LookGamepadAction,
			ETriggerEvent::Canceled,
			this,
			&AInputReceiver::LookGamepad
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LookGamepadAction is not set in InputReceiver."));
	}

	//インタラクトアクション
	if (IsValid(InteractAction))
	{
		EnhancedInputComponent->BindAction(
			InteractAction,
			ETriggerEvent::Started,
			this,
			&AInputReceiver::Interact
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InteractAction is not set in InputReceiver."));
	}

	//ダッシュアクション
	if (IsValid(DashAction))
	{
		EnhancedInputComponent->BindAction(
			DashAction,
			ETriggerEvent::Triggered,
			this,
			&AInputReceiver::Dash
		);

		EnhancedInputComponent->BindAction(
			DashAction,
			ETriggerEvent::Canceled,
			this,
			&AInputReceiver::StopDash
		);

		EnhancedInputComponent->BindAction(
			DashAction,
			ETriggerEvent::Completed,
			this,
			&AInputReceiver::StopDash
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DashAction is not set in InputReceiver."));
	}
}

//入力をInputManagerに渡す関数
void AInputReceiver::Move(const FInputActionValue& Value)
{
	InputManager->SetMoveInput(Value.Get<FVector2D>());
}

//マウス入力をInputManagerに渡す関数
void AInputReceiver::LookMouse(const FInputActionValue& Value)
{
	InputManager->SetMouseInput(Value.Get<FVector2D>());
}

//コントローラー入力をInputManagerに渡す関数
void AInputReceiver::LookGamepad(const FInputActionValue& Value)
{
	InputManager->SetGamepadLookInput(Value.Get<FVector2D>());
}

//インタラクション入力をInputManagerに渡す関数
void AInputReceiver::Interact()
{
	InputManager->SetIsInteracting(true);
}

//ダッシュ入力をInputManagerに渡す関数
void AInputReceiver::Dash()
{
	InputManager->SetIsDashing(true);
}

//ダッシュ入力が終了したときに呼ばれる関数
void AInputReceiver::StopDash()
{
	InputManager->SetIsDashing(false);
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/MDCharacterControlData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "../MakeDungeon.h"

AMDCharacterPlayer::AMDCharacterPlayer()
{
	AbilitySystemComponent = nullptr;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionKeyboardMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_KeyboardMove.IA_KeyboardMove'"));
	if (nullptr != InputActionKeyboardMoveRef.Object)
	{
		KeyboardMoveAction = InputActionKeyboardMoveRef.Object;
	}
}

void AMDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMDPlayerState* PS = GetPlayerState<AMDPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		for (const auto& StartInputAbility : InputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			AbilitySystemComponent->GiveAbility(StartSpec);

			MD_LOG(LogMD, Log, TEXT("%d, %s"), StartInputAbility.Key, *(StartInputAbility.Value)->GetAuthoredName());
		}

		//SetupGASInputComponent();

		/*APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));*/
	}

}

void AMDCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::MouseMove);
	EnhancedInputComponent->BindAction(KeyboardMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::KeyboardMove);

	//SetupGASInputComponent();
}

void AMDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMDCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		//EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::GASInputPressed, 0);
		//EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Canceled, this, &AMDCharacterPlayer::GASInputReleased, 0);
		//EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Completed, this, &AMDCharacterPlayer::GASInputReleased, 0);
		//EnhancedInputComponent->BindAction(KeyboardMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::KeyboardMove);

		MD_LOG(LogMD, Log, TEXT("Begin"));
	}
}

void AMDCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputPressed(*Spec);

			MD_LOG(LogMD, Log, TEXT("GASInputPressed_IsActive"));
		}
		else
		{
			AbilitySystemComponent->TryActivateAbility(Spec->Handle);
			MD_LOG(LogMD, Log, TEXT("GASInputPressed_IsNotActive"));
		}
	}
}

void AMDCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputReleased(*Spec);

			MD_LOG(LogMD, Log, TEXT("GASInputReleased_IsActive"));
		}
	}
}

void AMDCharacterPlayer::KeyboardMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

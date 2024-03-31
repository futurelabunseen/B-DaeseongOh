// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Data/MDInputData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Tags/MDGameplayTag.h"
#include "GameplayTagContainer.h"
#include "Character/MDCharacterBase.h"
#include "Item/MDWeaponBase.h"

AMDPlayerController::AMDPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AMDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(InputData->DefaultMappingContext, 0);
	}
}

void AMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(InputData->KeyboardMoveAction, ETriggerEvent::Triggered, this, &AMDPlayerController::KeyboardMove);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Started, this, &AMDPlayerController::OnMouseMoveStarted);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Triggered, this, &AMDPlayerController::OnMouseMoveTriggered);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Canceled, this, &AMDPlayerController::OnMouseMoveReleased);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Completed, this, &AMDPlayerController::OnMouseMoveReleased);

		EnhancedInputComponent->BindAction(InputData->WeaponSwapAction, ETriggerEvent::Started, this, &AMDPlayerController::SwapWeapon);
		

	}
}

void AMDPlayerController::KeyboardMove(const FInputActionValue& Value)
{
	StopMovement();

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
	SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	GetPawn()->AddMovementInput(MoveDirection, MovementVectorSize);
}

void AMDPlayerController::OnMouseMoveStarted()
{
	StopMovement();
}


void AMDPlayerController::OnMouseMoveTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMDPlayerController::OnMouseMoveReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void AMDPlayerController::SwapWeapon()
{
	AMDCharacterBase* MDChatacter = CastChecked<AMDCharacterBase>(GetPawn());

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if(SubSystem)
	{
		UMDWeaponBase* MDWeapon = MDChatacter->GetWeapon();

		if (!MDWeapon->GetWeaponAttackData() || MDTAG_WEAPON_TWOHANDEDSWORD != MDWeapon->GetWeaponAttackData()->WeaponType)
		{
			MDChatacter->SwapWeapon(MDTAG_WEAPON_TWOHANDEDSWORD);
		}
		else
		{
			MDChatacter->SwapWeapon(MDTAG_WEAPON_BOW);
		}
	}
}

void AMDPlayerController::GASInputStarted(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	ASC->TryActivateAbilitiesByTag(TagContainer);
}

void AMDPlayerController::GASInputPressed(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, AbilitiesToActivate);

	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		if(GameplayAbilitySpec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
		}
		else
		{
			ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
		}
	}
}

void AMDPlayerController::GASInputReleased(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, AbilitiesToActivate);

	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		ASC->AbilitySpecInputReleased(*GameplayAbilitySpec);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Data/MDInputData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Tags/MDGameplayTag.h"
#include "GameplayTagContainer.h"
#include "Character/MDCharacterPlayer.h"

//#include "Item/MDWeaponBase.h"

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
		SubSystem->AddMappingContext(InputData->WeaponMappingContext, 1);
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

		EnhancedInputComponent->BindAction(InputData->AttackAction, ETriggerEvent::Triggered, this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_PRIMARYATTACK);
		EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Triggered, this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_SKILL_01);
		EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Completed, this, &AMDPlayerController::GASInputReleased, MDTAG_WEAPON_SKILL_01);
		EnhancedInputComponent->BindAction(InputData->SkillAction_02, ETriggerEvent::Triggered, this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_SKILL_02);
		EnhancedInputComponent->BindAction(InputData->SkillAction_03, ETriggerEvent::Triggered, this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_SKILL_03);
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
	AMDCharacterPlayer* MDPlayer = CastChecked<AMDCharacterPlayer>(GetPawn());

	MDPlayer->SwapWeapon(MDTAG_WEAPON_TWOHANDEDSWORD);
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
	
	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			if (Spec.IsActive())
			{
				ASC->AbilitySpecInputPressed(Spec);
			}
			else
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}

}

void AMDPlayerController::GASInputReleased(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();

	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			if (Spec.IsActive())
			{
				ASC->AbilitySpecInputReleased(Spec);
			}
		}
	}
}

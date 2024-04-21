// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Item/MDWeaponBase.h"
#include "../MakeDungeon.h"


AMDCharacterPlayer::AMDCharacterPlayer()
{
	ASC = nullptr;
	AttributeSet = nullptr;

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
		ASC = PS->GetAbilitySystemComponent();
		AttributeSet = PS->GetAttributeSet();
		ASC->InitAbilityActorInfo(PS, this);

		for (const auto& StartAbility : CharacterAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : InputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			ASC->GiveAbility(StartSpec);
		}
	
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}

	Weapon->EquipWeapon(this);
}

void AMDCharacterPlayer::GASInputStarted(FGameplayTag Tag)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	ASC->TryActivateAbilitiesByTag(TagContainer);
}

void AMDCharacterPlayer::GASInputPressed(FGameplayTag Tag)
{
	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			if (Spec.IsActive())
			{
				ASC->AbilitySpecInputPressed(Spec);
				MD_LOG(LogMD, Log, TEXT("Pressed"));
			}
			else
			{
				ASC->TryActivateAbility(Spec.Handle);
				MD_LOG(LogMD, Log, TEXT("Activate"));
			}
		}
	}

	//If Input TagContainer
	/*FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, AbilitiesToActivate);

	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		if (GameplayAbilitySpec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
		}
		else
		{
			ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
		}
	}*/
}

void AMDCharacterPlayer::GASInputReleased(FGameplayTag Tag)
{
	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			if(Spec.IsActive())
			{
				ASC->AbilitySpecInputReleased(Spec);
				MD_LOG(LogMD, Log, TEXT("Released"));
			}
		}
	}

	//If Input TagContainer
	/*FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, AbilitiesToActivate);

	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		ASC->AbilitySpecInputReleased(*GameplayAbilitySpec);
	}*/
}

void AMDCharacterPlayer::StopMovement()
{
	GetController()->StopMovement();
}

void AMDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponBase.h"
#include "AbilitySystemComponent.h"
#include "Data/MDInputData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MDCharacterPlayer.h"
#include "Tags/MDGameplayTag.h"

UMDWeaponBase::UMDWeaponBase()
{
}

void UMDWeaponBase::EquipWeapon(AMDCharacterBase* InCharacter)
{
	UAbilitySystemComponent* ASC = InCharacter->GetAbilitySystemComponent();

	for (const auto& WeaponAbility : WeaponAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(WeaponAbility);
		ASC->GiveAbility(AbilitySpec);
	}

	// if InCharacte is Player
	if(AMDCharacterPlayer* CharacterPlayer = Cast<AMDCharacterPlayer>(InCharacter))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(CharacterPlayer->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputData->WeaponMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(InputData->AttackAction, ETriggerEvent::Triggered, CharacterPlayer, &AMDCharacterPlayer::GASInputPressed, MDTAG_INPUT_ATTACK);
				EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Triggered, CharacterPlayer, &AMDCharacterPlayer::GASInputPressed, MDTAG_INPUT_SKILL01);
			}
		}
	}
}

void UMDWeaponBase::UnequipWeapon(AMDCharacterBase* InCharacter)
{
	UAbilitySystemComponent* ASC = InCharacter->GetAbilitySystemComponent();

	for (const auto& WeaponAbility : WeaponAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(WeaponAbility);
		ASC->ClearAbility(AbilitySpec.Handle);
	}
}

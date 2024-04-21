// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponBase.h"
#include "AbilitySystemComponent.h"
#include "Data/MDWeaponInputData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MDCharacterPlayer.h"
#include "Tags/MDGameplayTag.h"

UMDWeaponBase::UMDWeaponBase()
{
}

void UMDWeaponBase::SetWeaponAttackData(AMDCharacterBase* InCharacter, UMDWeaponAttackData* WeaponData)
{


	WeaponAttackData = WeaponData;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InCharacter->GetMesh(), AttachmentRules, FName(TEXT("Weapon_R")));
	SetSkeletalMesh(WeaponAttackData->WeaponMesh);

	UAbilitySystemComponent* ASC = InCharacter->GetAbilitySystemComponent();

	for (const auto& WeaponAbility : WeaponAttackData->WeaponAbilities)
	{
		FGameplayAbilitySpec StartSpec(WeaponAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void UMDWeaponBase::EquipWeapon(AMDCharacterBase* InCharacter)
{
	UAbilitySystemComponent* ASC = InCharacter->GetAbilitySystemComponent();

	for (const auto& WeaponAbility : WeaponInputAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(WeaponAbility.Value);
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
				EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Triggered, CharacterPlayer, &AMDCharacterPlayer::GASInputPressed, MDTAG_WEAPON_SKILL_01);
				EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Completed, CharacterPlayer, &AMDCharacterPlayer::GASInputReleased, MDTAG_WEAPON_SKILL_01);
			}
		}
	}
}

void UMDWeaponBase::UnequipWeapon(AMDCharacterBase* InCharacter)
{
	UAbilitySystemComponent* ASC = InCharacter->GetAbilitySystemComponent();

	for (const auto& WeaponAbility : WeaponInputAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(WeaponAbility.Value);
		ASC->ClearAbility(AbilitySpec.Handle);
	}

	for (const auto& WeaponAbility : WeaponAttackData->WeaponAbilities)
	{
		FGameplayAbilitySpec StartSpec(WeaponAbility);
		ASC->ClearAbility(StartSpec.Handle);
	}
}

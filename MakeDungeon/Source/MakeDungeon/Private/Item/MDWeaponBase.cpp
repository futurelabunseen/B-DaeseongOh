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

void UMDWeaponBase::InitWeapon(AMDCharacterBase* InCharacter)
{
	UAbilitySystemComponent* ASC = InCharacter->GetAbilitySystemComponent();
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InCharacter->GetMesh(), AttachmentRules, FName(TEXT("Weapon_R")));

	for (auto& WeaponAbility : WeaponAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(WeaponAbility);
		ASC->GiveAbility(AbilitySpec);
	}
}

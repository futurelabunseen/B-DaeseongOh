// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Effects/MDGE_AttackDamage.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"

UMDGE_AttackDamage::UMDGE_AttackDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UMDCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMDCharacterAttributeSet, Health)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat DamageAmount(-30.f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);

	HealthModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(HealthModifier);
}

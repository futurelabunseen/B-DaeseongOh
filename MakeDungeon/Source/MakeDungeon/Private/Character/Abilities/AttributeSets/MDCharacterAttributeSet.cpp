// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"

UMDCharacterAttributeSet::UMDCharacterAttributeSet() : 
	AttackRange(100.f), MaxAttackRange(300.f), 
	AttackRadius(50.f), MaxAttackRadius(150.f),
	AttackRate(0.f), MaxAttackRate(100.f),
	Health(0.f), MaxHealth(100.f)
{
	InitHealth(GetMaxHealth());
}

void UMDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UMDCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
}

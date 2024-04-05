// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "../MakeDungeon.h"

UMDCharacterAttributeSet::UMDCharacterAttributeSet() : 
	AttackRange(100.f), MaxAttackRange(300.f), 
	AttackRadius(50.f), MaxAttackRadius(150.f),
	AttackRate(10.f), MaxAttackRate(100.f),
	Health(0.f), MaxHealth(100.f)
{
	InitHealth(GetMaxHealth());
}

void UMDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UMDCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		MD_LOG(LogMD, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}

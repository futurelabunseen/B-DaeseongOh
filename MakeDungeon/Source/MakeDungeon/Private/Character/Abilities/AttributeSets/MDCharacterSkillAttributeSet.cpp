// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/MDCharacterSkillAttributeSet.h"

UMDCharacterSkillAttributeSet::UMDCharacterSkillAttributeSet() : 
	SkillRange(0.1f), MaxSkillRange(1000.f),
	SkillAttackRate(0.f), MaxSkillAttackRate(200.f),
	SkillEnergy(25.f), MaxSkillEnergy(50.f)
{
}

void UMDCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetSkillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if (Attribute == GetMaxSkillAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSkillAttackRate());
	}
}

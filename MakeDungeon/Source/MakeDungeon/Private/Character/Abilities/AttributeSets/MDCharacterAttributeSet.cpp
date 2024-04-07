// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "../MakeDungeon.h"

UMDCharacterAttributeSet::UMDCharacterAttributeSet() : 
	AttackRange(100.f), MaxAttackRange(300.f), 
	AttackRadius(50.f), MaxAttackRadius(150.f),
	AttackRate(10.f), MaxAttackRate(100.f),
	Health(0.f), MaxHealth(100.f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
}

void UMDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}
}

void UMDCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinHealth = 0.f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		MD_LOG(LogMD, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		MD_LOG(LogMD, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.f);
	}
}

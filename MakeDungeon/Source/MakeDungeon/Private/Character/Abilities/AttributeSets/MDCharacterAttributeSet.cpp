// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDCharacterAttributeSet::UMDCharacterAttributeSet() : 
	AttackRange(100.f), MaxAttackRange(300.f), 
	AttackRadius(100.f), MaxAttackRadius(200.f),
	AttackRate(10.f), MaxAttackRate(100.f),
	Health(0.f), MaxHealth(100.f),
	Mana(0.f), MaxMana(100.f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
	InitMana(GetMaxMana());
}

void UMDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}
}

bool UMDCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.f)
		{
			if (Data.Target.HasMatchingGameplayTag(MDTAG_CHARACTER_INVINCIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.f;
				return false;
			}
		}
	}

	return true;
}

void UMDCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinHealth = 0.f;
	float MinMana = 0.f;

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
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), MinMana, GetMaxMana()));
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(MDTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.f);
}

void UMDCharacterAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	float MinHealth = 0.f;
	float MinMana = 0.f;

	if (Attribute == GetHealthAttribute() && MaxHealth.GetBaseValue() < Health.GetBaseValue())
	{
	}
}

void UMDCharacterAttributeSet::Revive()
{
	bOutOfHealth = false;
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());
}

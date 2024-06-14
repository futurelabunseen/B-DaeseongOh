// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Abilities/AttributeSets/MDWeaponAttributeSet.h"

UMDWeaponAttributeSet::UMDWeaponAttributeSet() :
	WeaponRange(50.f), MaxWeaponRange(100.f),
	WeaponRadius(0.f), MaxWeaponRadius(100.f),
	WeaponAttackRate(10.f), MaxWeaponAttackRate(100.f)
{
}

void UMDWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

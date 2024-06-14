// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MDWeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MAKEDUNGEON_API UMDWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMDWeaponAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	//virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(UMDWeaponAttributeSet, WeaponRange);
	ATTRIBUTE_ACCESSORS(UMDWeaponAttributeSet, MaxWeaponRange);
	ATTRIBUTE_ACCESSORS(UMDWeaponAttributeSet, WeaponRadius);
	ATTRIBUTE_ACCESSORS(UMDWeaponAttributeSet, MaxWeaponRadius);
	ATTRIBUTE_ACCESSORS(UMDWeaponAttributeSet, WeaponAttackRate);
	ATTRIBUTE_ACCESSORS(UMDWeaponAttributeSet, MaxWeaponAttackRate);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxWeaponRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxWeaponRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxWeaponAttackRate;
};


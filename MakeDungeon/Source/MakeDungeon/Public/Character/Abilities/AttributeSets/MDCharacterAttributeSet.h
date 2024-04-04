// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MDCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMDCharacterAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, AttackRange);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, MaxAttackRange);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, AttackRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, MaxAttackRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, AttackRate);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, MaxAttackRate);

	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMDCharacterAttributeSet, MaxHealth);
};

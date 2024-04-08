// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MDCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UMDCharacterAttributeSet;

UCLASS()
class MAKEDUNGEON_API AMDCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMDCharacterBase();

	//FORCEINLINE virtual UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	//FORCEINLINE class UMDAttackMontageData* GetAttackMontageData() const { return AttackMontageData; }
	FORCEINLINE FGameplayTag GetWeaponType() const { return CurrentWeaponType; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE void SetWeaponType(FGameplayTag Tag) { CurrentWeaponType = Tag; }

protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UMDWeaponAttackData> AttackMontageData;*/

	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	UPROPERTY()
	TObjectPtr<UMDCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag CurrentWeaponType;
};

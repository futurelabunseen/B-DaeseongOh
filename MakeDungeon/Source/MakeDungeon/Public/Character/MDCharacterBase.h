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
class UMDWeaponBase;

UCLASS()
class MAKEDUNGEON_API AMDCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMDCharacterBase();

	virtual void BeginPlay() override;

	//FORCEINLINE virtual UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	//FORCEINLINE class UMDAttackMontageData* GetAttackMontageData() const { return AttackMontageData; }
	FORCEINLINE UMDWeaponBase* GetWeapon() const { return Weapon; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void SwapWeapon(FGameplayTag Tag);

	virtual void StopMovement() {}

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
	TObjectPtr<UMDWeaponBase> Weapon;
};

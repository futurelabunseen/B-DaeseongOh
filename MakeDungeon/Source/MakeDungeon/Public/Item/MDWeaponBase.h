// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Data/MDComboAttackData.h"
#include "MDWeaponBase.generated.h"

class UGameplayAbility;
class AMDCharacterBase;
class UMDWeaponInputData;
class UInputMappingContext;

UENUM()
enum class EMDAttackType : uint8
{
	None			UMETA(DisplayName = "None"),
	PrimaryAttack	UMETA(DisplayName = "PrimaryAttack"), 
	Skill_01		UMETA(DisplayName = "Skill_01"), 
	Skill_02		UMETA(DisplayName = "Skill_02"), 
	Skill_03		UMETA(DisplayName = "Skill_03"), 
	Skill_04		UMETA(DisplayName = "Skill_04")
};

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDWeaponBase : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UMDWeaponBase();

	FORCEINLINE UMDComboAttackData* GetComboAttackData(EMDAttackType AttackIndex = EMDAttackType::PrimaryAttack) const { return ComboAttackData[AttackIndex]; }
	FORCEINLINE UAnimMontage* GetMontage(EMDAttackType AttackIndex = EMDAttackType::PrimaryAttack) const { return Montage[AttackIndex]; }
	FORCEINLINE FGameplayTag GetWeaponType() const { return WeaponType; }
	FORCEINLINE UInputMappingContext* GetMappingContext() const { return WeaponMappingContext; }
	void InitWeapon(AMDCharacterBase* InCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<EMDAttackType, TObjectPtr<UAnimMontage>> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<EMDAttackType, TObjectPtr<UMDComboAttackData>> ComboAttackData;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> WeaponInputAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	FGameplayTag WeaponType;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UMDWeaponInputData> InputData;
};

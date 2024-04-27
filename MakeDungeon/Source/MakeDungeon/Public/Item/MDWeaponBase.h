// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Data/MDWeaponAttackData.h"
#include "MDWeaponBase.generated.h"

class UGameplayAbility;
class AMDCharacterBase;
class UMDWeaponInputData;

enum class EMDMontage : uint8
{
	PrimaryAttack, Skill_01, Skill_02, Skill_03, Skill_04
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

	//FORCEINLINE UAnimMontage* GetAttackMontage() const { return WeaponAttackData->AttackMontage; }
	//FORCEINLINE UMDWeaponAttackData* GetWeaponAttackData() const { return WeaponAttackData; }
	FORCEINLINE UAnimMontage* GetMontage(EMDMontage MontageIndex = EMDMontage::PrimaryAttack) const { return Montage[static_cast<uint8>(MontageIndex)]; }
	FORCEINLINE FGameplayTag GetWeaponType() { return WeaponType; }
	//void SetWeaponAttackData(AMDCharacterBase* InCharacter, UMDWeaponAttackData* WeaponData);
	virtual void InitializeComponent() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> Montage;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UMDWeaponAttackData> WeaponAttackData;*/

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

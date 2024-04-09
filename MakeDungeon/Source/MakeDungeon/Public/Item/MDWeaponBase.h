// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Data/MDWeaponAttackData.h"
#include "MDWeaponBase.generated.h"

class UGameplayAbility;
class AMDCharacterBase;
class UMDInputData;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDWeaponBase : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UMDWeaponBase();

	FORCEINLINE UAnimMontage* GetAttackMontage() const { return WeaponAttackData->AttackMontage; }
	FORCEINLINE UMDWeaponAttackData* GetWeaponAttackData() const { return WeaponAttackData; }
	FORCEINLINE void SetWeaponAttackData(UMDWeaponAttackData* WeaponData) { WeaponAttackData = WeaponData; }

	void EquipWeapon(AMDCharacterBase* InCharacter);
	void UnequipWeapon(AMDCharacterBase* InCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UMDWeaponAttackData> WeaponAttackData;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UMDInputData> InputData;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Data/MDWeaponAttackData.h"
#include "MDWeaponBase.generated.h"

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

	virtual void EquipWeapon();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UMDWeaponAttackData> WeaponAttackData;

private:
};

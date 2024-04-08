// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MDWeaponBase.h"
#include "MDWeaponSword.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = Weapon, meta = (BlueprintSpawnableComponent))
class MAKEDUNGEON_API UMDWeaponSword : public UMDWeaponBase
{
	GENERATED_BODY()

public:
	UMDWeaponSword();

	virtual void EquipWeapon() override;
};

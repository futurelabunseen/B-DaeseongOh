// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponSword.h"
#include "Character/MDCharacterBase.h"

UMDWeaponSword::UMDWeaponSword()
{
	static ConstructorHelpers::FObjectFinder<UMDWeaponAttackData> WeaponAttackDataRef(
		TEXT("/Script/MakeDungeon.MDWeaponAttackData'/Game/MakeDungeon/Data/MD_Attack_TwoHandedSword.MD_Attack_TwoHandedSword'"));
	if (nullptr != WeaponAttackDataRef.Object)
	{
		WeaponAttackData = WeaponAttackDataRef.Object;
	}
}

void UMDWeaponSword::EquipWeapon()
{
	Super::EquipWeapon();
}

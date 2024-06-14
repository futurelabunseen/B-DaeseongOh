// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponTwoHandedSword.h"

UMDWeaponTwoHandedSword::UMDWeaponTwoHandedSword()
{
}

void UMDWeaponTwoHandedSword::InitWeapon(AMDCharacterBase* InCharacter)
{
	Super::InitWeapon(InCharacter);

	AddLocalRotation(FRotator(0.0, 0.0, 180.0));
	AddLocalOffset(FVector(0.0, 0.0, -183.0));
}

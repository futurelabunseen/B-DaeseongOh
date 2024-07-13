// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponTwoHandedSword.h"
#include "Character/MDCharacterBase.h"

UMDWeaponTwoHandedSword::UMDWeaponTwoHandedSword()
{
}

void UMDWeaponTwoHandedSword::InitWeapon(AMDCharacterBase* InCharacter)
{
	Super::InitWeapon(InCharacter);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InCharacter->GetMesh(), AttachmentRules, FName(TEXT("weapon_r")));

	AddLocalRotation(FRotator(0.0, 90.0, 0.0));
	AddLocalOffset(FVector(0.0, 10.0, -30.0));
}

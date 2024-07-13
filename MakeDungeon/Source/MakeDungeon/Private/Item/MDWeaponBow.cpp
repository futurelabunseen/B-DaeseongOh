// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponBow.h"
#include "Character/MDCharacterBase.h"

UMDWeaponBow::UMDWeaponBow()
{
}

void UMDWeaponBow::InitWeapon(AMDCharacterBase* InCharacter)
{
	Super::InitWeapon(InCharacter);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InCharacter->GetMesh(), AttachmentRules, FName(TEXT("weapon_l")));

	AddLocalRotation(FRotator(0.0, -90.0, 0.0));
	AddLocalOffset(FVector(0.0, 10.0, 0.0));
}

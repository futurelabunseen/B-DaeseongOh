// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MDWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"

void AMDWeaponBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, WeaponEventTag, FGameplayEventData());
}

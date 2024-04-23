// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_Sword_LeapAttack.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_Sword_LeapAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMDGA_Sword_LeapAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnCompletedCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UPROPERTY()
	TObjectPtr<class UMDWeaponAttackData> CurrentWeaponAttackData;
};

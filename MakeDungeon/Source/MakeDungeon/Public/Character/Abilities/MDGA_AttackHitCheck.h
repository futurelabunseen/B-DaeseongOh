// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UMDGA_AttackHitCheck();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};

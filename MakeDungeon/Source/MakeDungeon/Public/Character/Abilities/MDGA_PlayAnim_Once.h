// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_PlayAnim_Once.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_PlayAnim_Once : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMDGA_PlayAnim_Once();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnInterrupted();
};

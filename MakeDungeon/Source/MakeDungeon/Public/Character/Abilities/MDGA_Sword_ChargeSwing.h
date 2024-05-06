// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_Sword_ChargeSwing.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_Sword_ChargeSwing : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMDGA_Sword_ChargeSwing();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TriggerGameplayTag;

private:
	float Radius;
	FVector SpawnLocation;
};

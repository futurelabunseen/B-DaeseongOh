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
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

protected:
	// For Anim
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimPlaySpeed = 0.5f;

	// For Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TriggerGameplayTag;

	// For Effect
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> SkillInitEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> SkillUpdateEffect;

private:
	// For Anim
	FTimerHandle AttackCheckTimerHandle;

	// For Attack
	float Radius;
	FVector SpawnLocation;
};

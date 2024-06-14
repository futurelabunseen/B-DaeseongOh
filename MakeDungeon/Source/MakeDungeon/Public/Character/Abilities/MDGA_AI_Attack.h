// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_AI_Attack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityExcuted);
/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_AI_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMDGA_AI_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnInterrupted();

public:
	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityExcuted OnAbilityExecuted;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> Montage;
};

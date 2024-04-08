// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMDGA_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompletedCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection(bool bIsSecondary = false);
	void StartComboTimer();
	void CheckComboInput();	

protected:
	UPROPERTY()
	TObjectPtr<class UMDWeaponAttackData> CurrentWeaponAttackData;

	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextAttackInput = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "MDGA_AttackHitCheck.generated.h"

class UGameplayEffect;
class AMDTA_Trace;
class UNiagaraSystem;

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

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackBuffEffect;

	float CurrentLevel;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<AMDTA_Trace> TargetActorClass;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> TargetDebuffEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TObjectPtr<UNiagaraSystem> NiagaraFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	float VFXScale;

	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag HitEffectTag;
};

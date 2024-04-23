// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Character/MDProjectile.h"
#include "MDGA_Bow_MultiShot.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_Bow_MultiShot : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMDGA_Bow_MultiShot();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMDProjectile> ProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:


private:
	float Range;
	double OuterAngle;
	double DecreaseAngle;
	FName SocketName;
};
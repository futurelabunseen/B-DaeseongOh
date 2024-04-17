// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Character/MDProjectile.h"
#include "MDGA_AttackRanged.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_AttackRanged : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMDGA_AttackRanged();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMDProjectile> ProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void ShootBullet(FGameplayEventData EventData);

private:
	float Range;

	FName SocketName;
};

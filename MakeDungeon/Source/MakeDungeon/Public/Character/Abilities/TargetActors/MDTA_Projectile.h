// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "MDTA_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDTA_Projectile : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AMDTA_Projectile();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool IsShowDebug) { bShowDebug = IsShowDebug; }
protected:

	bool bShowDebug = false;
};

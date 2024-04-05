// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "MDTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	AMDTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool IsShowDebug) { bShowDebug = IsShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};

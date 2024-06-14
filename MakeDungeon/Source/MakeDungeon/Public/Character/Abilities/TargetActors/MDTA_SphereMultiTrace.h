// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Abilities/TargetActors/MDTA_Trace.h"
#include "MDTA_SphereMultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDTA_SphereMultiTrace : public AMDTA_Trace
{
	GENERATED_BODY()
	
protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "MDGC_Debuff_Base.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDGC_Debuff_Base : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
public:
	AMDGC_Debuff_Base();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayCue")
	TObjectPtr<UParticleSystem> ParticleSystem;
};

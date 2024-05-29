// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "MDGC_Debuff_Base.generated.h"

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDGC_Debuff_Base : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
public:
	AMDGC_Debuff_Base();

	//virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters);
	//virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters); bool OnRemove(AActor* MyTarget, const FGameplayCueParameters& Parameters);

protected:

public:

protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayCue")
	TObjectPtr<UNiagaraComponent> NiagaraFX;*/
};

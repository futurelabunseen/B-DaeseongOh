// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "MDGC_AttackHit.generated.h"

class USoundBase;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGC_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UMDGC_AttackHit();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayCue")
	TObjectPtr<USoundBase> SoundFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayCue")
	TObjectPtr<UNiagaraSystem> NiagaraFX;
};

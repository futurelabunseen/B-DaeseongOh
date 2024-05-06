// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MDCharacterBase.h"
#include "MDCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDCharacterNonPlayer : public AMDCharacterBase
{
	GENERATED_BODY()
public:
	AMDCharacterNonPlayer();

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void OnOutOfHealth() override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	float Level;
};

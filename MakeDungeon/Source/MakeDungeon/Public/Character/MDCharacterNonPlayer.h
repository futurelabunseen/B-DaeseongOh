// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MDCharacterBase.h"
#include "Interface/MDCharacterAIInterface.h"
#include "MDCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDCharacterNonPlayer : public AMDCharacterBase, public IMDCharacterAIInterface
{
	GENERATED_BODY()
public:
	AMDCharacterNonPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void SetDead() override;

	virtual void OnOutOfHealth() override;

	// AI Section
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	
	UFUNCTION()
	void AttackAIFinish();

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	float Level;

	FAICharacterAttackFinished OnAttackFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AttackTriggerGameplayTag;

private:
	float DeadTime = 0.f;

};

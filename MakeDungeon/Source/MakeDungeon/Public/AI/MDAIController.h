// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MDAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMDAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};

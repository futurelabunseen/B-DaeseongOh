// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidgetBase.h"
#include "MDGameScoreWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGameScoreWidget : public UMDUserWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void UpdateWidget() override;

	void SetScore(int32 NewScore);
	void SetSpawnTime(int32 NewTime);

protected:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtRemainingTarget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtRemainingSpawnTime;

	int32 RemainingTarget = 0;
	int32 RemainingSpawnTime = 0;
};

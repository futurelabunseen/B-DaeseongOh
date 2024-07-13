// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MDPierceCountWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDPierceCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetCount(int32 NewCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtCount;

	int32 Count;
};

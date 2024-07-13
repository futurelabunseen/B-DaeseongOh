// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MDUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void UpdateWidget() {}
};

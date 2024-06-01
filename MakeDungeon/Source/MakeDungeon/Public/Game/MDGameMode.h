// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MDGameMode.generated.h"

class UMDLogoWidget;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMDGameMode();

protected:
	virtual void BeginPlay() override;

protected:
	TSubclassOf<UMDLogoWidget> LogoWidgetClass;
	TObjectPtr<UMDLogoWidget> LogoWidget;
};

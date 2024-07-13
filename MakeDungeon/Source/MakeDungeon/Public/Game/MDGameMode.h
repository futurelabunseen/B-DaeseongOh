// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/MDGameInterface.h"
#include "MDGameMode.generated.h"

class UMDLogoWidget;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDGameMode : public AGameMode, public IMDGameInterface
{
	GENERATED_BODY()
	
public:
	AMDGameMode();

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

protected:
	virtual void BeginPlay() override;

	FString GetCurrentLevelName();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	int32 ClearScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Game")
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Game")
	uint8 bIsCleared : 1;

protected:
	TSubclassOf<UMDLogoWidget> LogoWidgetClass;
	TObjectPtr<UMDLogoWidget> LogoWidget;
};

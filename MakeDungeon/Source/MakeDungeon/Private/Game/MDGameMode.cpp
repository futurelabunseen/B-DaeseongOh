// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MDGameMode.h"
#include "Player/MDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MDLogoWidget.h"
#include "../MakeDungeon.h"

AMDGameMode::AMDGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MakeDungeon/Blueprints/BP_MDCharacterPlayer.BP_MDCharacterPlayer_C"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/MakeDungeon/Blueprints/BP_MDPlayerController.BP_MDPlayerController_C"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UMDLogoWidget> LogoWidgetRef(TEXT(
										"/Game/MakeDungeon/UI/WBP_Logo.WBP_Logo_C"));
	if (LogoWidgetRef.Class)
	{
		LogoWidgetClass = LogoWidgetRef.Class;
	}

	ClearScore = 0;
	CurrentScore = 0;
	bIsCleared = false;
}

void AMDGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore += NewPlayerScore;

	AMDPlayerController* MDPlayerController = 
			Cast<AMDPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MDPlayerController)
	{
		MDPlayerController->GameScoreChanged(CurrentScore);
	}

	if (ClearScore >= CurrentScore)
	{
		bIsCleared = true;



		if (MDPlayerController)
		{
			MDPlayerController->GameClear();
		}
	}
}

void AMDGameMode::OnPlayerDead()
{
	AMDPlayerController* MDPlayerController = 
			Cast<AMDPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MDPlayerController)
	{
		MDPlayerController->GameOver();
	}
}

bool AMDGameMode::IsGameCleared()
{
	return bIsCleared;
}

void AMDGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GetCurrentLevelName() == TEXT("Logo"))
	{
		if (IsValid(LogoWidgetClass))
		{
			LogoWidget = Cast<UMDLogoWidget>(CreateWidget(GetWorld(), LogoWidgetClass));
			if (IsValid(LogoWidget))
			{
				LogoWidget->AddToViewport();
			}
		}
	}
}

FString AMDGameMode::GetCurrentLevelName()
{
	FString LevelNameWithPath = GetWorld()->GetMapName();
	int32 LastUnderbarIndex;
	LevelNameWithPath.FindLastChar('_', LastUnderbarIndex);
	
	return LevelNameWithPath.Mid(LastUnderbarIndex + 1);
}

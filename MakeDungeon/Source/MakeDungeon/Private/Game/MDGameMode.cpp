// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MDGameMode.h"
#include "Player/MDPlayerController.h"
#include "UI/MDLogoWidget.h"

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
}

void AMDGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LogoWidgetClass))
	{
		LogoWidget = Cast<UMDLogoWidget>(CreateWidget(GetWorld(), LogoWidgetClass));
		if (IsValid(LogoWidget))
		{
			LogoWidget->AddToViewport();
		}
	}
}

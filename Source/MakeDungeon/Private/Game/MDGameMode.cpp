// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MDGameMode.h"
#include "Player/MDPlayerController.h"

AMDGameMode::AMDGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MakeDungeon/Blueprints/BP_MDCharacterPlayer.BP_MDCharacterPlayer_C"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/MakeDungeon/Blueprints/BP_MDPlayerController.BP_MDPlayerController_C"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

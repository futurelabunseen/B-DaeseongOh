// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterMonster.h"
#include "AI/MDAIController.h"

AMDCharacterMonster::AMDCharacterMonster()
{
	AIControllerClass = AMDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

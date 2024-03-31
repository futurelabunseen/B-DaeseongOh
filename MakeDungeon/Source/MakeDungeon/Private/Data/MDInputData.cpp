// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MDInputData.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"


UMDInputData::UMDInputData()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MakeDungeon/Input/IMC_Default.IMC_Default'"));
	if (nullptr != DefaultMappingContextRef.Object)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionKeyboardMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_KeyboardMove.IA_KeyboardMove'"));
	if (nullptr != InputActionKeyboardMoveRef.Object)
	{
		KeyboardMoveAction = InputActionKeyboardMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMouseMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_SetDestination_Click.IA_SetDestination_Click'"));
	if (nullptr != InputActionMouseMoveRef.Object)
	{
		MouseMoveAction = InputActionMouseMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		KeyboardMoveAction = InputActionAttackRef.Object;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "../MakeDungeon.h"

AMDPlayerController::AMDPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMouseMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_SetDestination_Click.IA_SetDestination_Click'"));
	if (nullptr != InputActionMouseMoveRef.Object)
	{
		MouseMoveAction = InputActionMouseMoveRef.Object;
	}
}

void AMDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &AMDPlayerController::OnMouseMoveTriggered);
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Canceled, this, &AMDPlayerController::OnMouseMoveReleased);
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Completed, this, &AMDPlayerController::OnMouseMoveReleased);
	}
}

void AMDPlayerController::OnMouseMoveTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}

	MD_LOG(LogMD, Log, TEXT("MouseMove Triggered"));
}

void AMDPlayerController::OnMouseMoveReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

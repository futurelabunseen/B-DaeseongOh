// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Data/MDInputData.h"
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
}

void AMDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(InputData->DefaultMappingContext, 0);
	}
}

void AMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Triggered, this, &AMDPlayerController::OnMouseMoveTriggered);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Canceled, this, &AMDPlayerController::OnMouseMoveReleased);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Completed, this, &AMDPlayerController::OnMouseMoveReleased);
		EnhancedInputComponent->BindAction(InputData->KeyboardMoveAction, ETriggerEvent::Triggered, this, &AMDPlayerController::KeyboardMove);
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

void AMDPlayerController::KeyboardMove(const FInputActionValue& Value)
{
	StopMovement();

	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	GetPawn()->AddMovementInput(MoveDirection, MovementVectorSize);
}

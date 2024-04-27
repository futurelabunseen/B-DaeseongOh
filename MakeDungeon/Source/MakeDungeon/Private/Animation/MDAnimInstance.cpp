// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MDAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MDCharacterBase.h"

UMDAnimInstance::UMDAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 100.f;
	AnimPlaySpeed = 1.f;
}

void UMDAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* Temp = GetOwningActor();
	Owner = Cast<AMDCharacterBase>(Temp);
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMDAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
	if (Owner)
	{
		bIsCharging = Owner->IsCharged();
		WeaponType = Owner->GetWeaponType();
	}
}

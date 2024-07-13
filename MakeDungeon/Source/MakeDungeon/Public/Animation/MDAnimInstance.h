// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "MDAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMDAnimInstance();

	FORCEINLINE void SetAnimPlaySpeed(float AnimSpeed = 1.f) { AnimPlaySpeed = AnimSpeed; }
	FORCEINLINE void SetWeaponType(FGameplayTag Type) { WeaponType = Type; }

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AMDCharacterBase> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FGameplayTag WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	FGameplayTagContainer CurrentOwnedTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsCharging : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsLooping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float AnimPlaySpeed;
};

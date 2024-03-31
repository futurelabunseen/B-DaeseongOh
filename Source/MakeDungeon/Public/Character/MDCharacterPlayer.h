// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MDCharacterBase.h"
#include "InputActionValue.h"
#include "MDCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDCharacterPlayer : public AMDCharacterBase
{
	GENERATED_BODY()
	
public:
	AMDCharacterPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

// Character Control Section
protected:
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);
	

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> KeyboardMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	void KeyboardMove(const FInputActionValue& Value);
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> InputAbilities;
};

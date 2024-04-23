// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MDCharacterBase.h"
//#include "GameplayTagContainer.h"
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

	virtual FRotator GetAttackDirection() const override;

	void GASInputStarted(FGameplayTag Tag);
	void GASInputPressed(FGameplayTag Tag);
	void GASInputReleased(FGameplayTag Tag);
	
	virtual void StopMovement() override;

protected:
	virtual void BeginPlay() override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> InputAbilities;
};

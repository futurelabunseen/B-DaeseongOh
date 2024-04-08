// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MDPlayerController.generated.h"

struct FInputActionValue;
class UMDInputData;
struct FGameplayTag;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMDPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UNiagaraSystem* FXCursor;

protected:
	virtual void SetupInputComponent() override;

	void KeyboardMove(const FInputActionValue& Value);
	void OnMouseMoveStarted();
	void OnMouseMoveTriggered();
	void OnMouseMoveReleased();

	void SwapWeapon();

	void GASInputStarted(FGameplayTag Tag);
	void GASInputPressed(FGameplayTag Tag);
	void GASInputReleased(FGameplayTag Tag);

	uint32 bMoveToMouseCursor : 1;

	FVector CachedDestination;

	float FollowTime;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UMDInputData> InputData;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MDPlayerController.generated.h"

class UMDInputData;
struct FInputActionValue;
class UAbilitySystemComponent;
struct FGameplayTag;
class UMDHUDWidget;

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

protected:
	virtual void SetupInputComponent() override;

	void KeyboardMove(const FInputActionValue& Value);
	void OnMouseMoveStarted();
	void OnMouseMoveTriggered();
	void OnMouseMoveReleased();
	void OnCameraMove(const FInputActionValue& Value);
	void OffCameraMove(const FInputActionValue& Value);
	void OnCameraRotate(const FInputActionValue& Value);
	void OnCameraZoom(const FInputActionValue& Value);

	void SwapWeapon();

	void GASInputStarted(FGameplayTag Tag);
	void GASInputPressed(FGameplayTag Tag);
	void GASInputReleased(FGameplayTag Tag);

	bool IsMatchLevel(const FString& InLevelName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UNiagaraSystem> FXCursor;


protected:
	uint32 bMoveToMouseCursor : 1;
	uint8	bIsCameraMove : 1;

	FVector CachedDestination;

	float FollowTime;

// HUD Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UMDHUDWidget> MDHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<UMDHUDWidget> MDHUDWidget;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UMDInputData> InputData;

};

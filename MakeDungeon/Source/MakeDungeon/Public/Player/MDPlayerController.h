// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "UI/MDUIType.h"
#include "MDPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionDelegate);

class UMDInputData;
struct FInputActionValue;
class UAbilitySystemComponent;
class UMDHUDWidget;
class UMDGameScoreWidget;
class UMDUserWidget;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMDPlayerController();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);
	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();
	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();

	void SpawnTimeChanged(int32 NewTime);


	UFUNCTION(BlueprintCallable)
	UMDUserWidgetBase* GetUI(EMDUIType UIType);

	void HideUI(EMDUIType UIType);

	UFUNCTION()
	void VisibleShop();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void CreateHUD();

	void KeyboardMove(const FInputActionValue& Value);
	void OnMouseMoveStarted();
	void OnMouseMoveTriggered();
	void OnMouseMoveReleased();
	void OnCameraMove(const FInputActionValue& Value);
	void OffCameraMove(const FInputActionValue& Value);
	void OnCameraRotate(const FInputActionValue& Value);
	void OnCameraZoom(const FInputActionValue& Value);
	void ResetGame();

	void Interaction();
	void SwapWeapon();
	void VisibleInventory();

	void AddItem_Test();

	void GASInputStarted(FGameplayTag Tag);
	void GASInputPressed(FGameplayTag Tag);
	void GASInputReleased(FGameplayTag Tag);

	bool IsMatchLevel(const FString& InLevelName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UNiagaraSystem> FXCursor;

	FInteractionDelegate OnInteraction;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UMDGameScoreWidget> MDGameScoreWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ClearWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> ClearWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> OverWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> OverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TMap<EMDUIType, TSubclassOf<UUserWidget>> HUDWidgetClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	TMap<EMDUIType, TObjectPtr<UUserWidget>> HUDWidgets;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UMDInputData> InputData;

	//ForDebugTest
	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UUserWidget> TestItemClass;
};

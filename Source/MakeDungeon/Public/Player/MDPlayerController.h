// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MDPlayerController.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseMoveAction;
protected:
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	void OnMouseMoveTriggered();
	void OnMouseMoveReleased();

private:
	FVector CachedDestination;

	float FollowTime;

};

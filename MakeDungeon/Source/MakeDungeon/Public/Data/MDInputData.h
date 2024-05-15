// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MDInputData.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDInputData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMDInputData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> KeyboardMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MouseMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraRotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WeaponSwapAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|PlayerDefault", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction_01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction_02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction_03;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction_04;
};

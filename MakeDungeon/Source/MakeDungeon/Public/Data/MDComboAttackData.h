// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "MDComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMDComboAttackData();

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "InputThreshold")
	float InputThreshold;
};

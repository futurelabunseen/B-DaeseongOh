// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MDAttackMontageData.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDAttackMontageData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMDAttackMontageData();

	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Name")
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "InputThreshold")
	float InputThreshold;
};

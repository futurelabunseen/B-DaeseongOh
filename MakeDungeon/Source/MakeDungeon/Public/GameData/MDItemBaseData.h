// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MDItemBaseData.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDItemBaseData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMDItemBaseData();

	UPROPERTY(EditAnywhere)
	FString ItemName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ItemIcon;
};

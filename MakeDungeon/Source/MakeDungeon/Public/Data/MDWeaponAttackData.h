// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MDWeaponAttackData.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDWeaponAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMDWeaponAttackData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "InputThreshold")
	float InputThreshold;

	UPROPERTY(EditAnywhere, Category = "Tag")
	FGameplayTag WeaponType;


};

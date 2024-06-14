// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MDItemBox.h"
#include "MDWeaponBox.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDWeaponBox : public AMDItemBox
{
	GENERATED_BODY()
	
public:
	virtual void NotifyActorBeginOverlap(AActor* Other) override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS", meta = (Categories = Event))
	FGameplayTag WeaponEventTag;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidgetBase.h"
#include "MDInventory.generated.h"

class UTextBlock;
class UTileView;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDInventory : public UMDUserWidgetBase
{
	GENERATED_BODY()
	
public:
	void UpdateItemTile();

	UFUNCTION(BlueprintCallable)
	void AddItem(UObject* Item);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtInventoryName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> ItemTile;

};

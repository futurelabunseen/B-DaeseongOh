// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidgetBase.h"
#include "MDInventory.generated.h"

class UTextBlock;
class UTileView;
class UMDItemBaseData;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDInventory : public UMDUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void BuyItem(UMDItemBaseData* InItem);

	UFUNCTION(BlueprintCallable)
	void SellItem(UMDItemBaseData* InItem);

	void UpdateItemTile();

	const TArray<UObject*> GetAllItems();

	UFUNCTION(BlueprintCallable)
	void AddItem(UMDItemBaseData* InItem);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtInventoryName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> ItemTile;

};

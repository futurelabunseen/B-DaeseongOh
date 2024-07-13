// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "MDInventory_Item.generated.h"

class UImage;
class UTextBlock;
class UTileView;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDInventory_Item : public UMDUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImgItem;

};

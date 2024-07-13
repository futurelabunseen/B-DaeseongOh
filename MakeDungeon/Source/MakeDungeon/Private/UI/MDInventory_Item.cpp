// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDInventory_Item.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TileView.h"
#include "GameData/MDItemBaseData.h"

void UMDInventory_Item::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMDInventory_Item::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UMDItemBaseData* ItemData = Cast<UMDItemBaseData>(ListItemObject);

	if (ItemData)
	{
		TxtItemName->SetText(FText::FromString(ItemData->ItemName));

		if (ItemData->ItemIcon)
		{
			ImgItem->SetBrushFromTexture(ItemData->ItemIcon);
		}
	}
}

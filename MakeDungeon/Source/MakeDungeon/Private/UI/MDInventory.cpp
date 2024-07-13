// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDInventory.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"

void UMDInventory::UpdateItemTile()
{
	for (int32 i = 0; i < 30; i++)
	{

		//ItemTile->AddItem()
		//ItemTile->RemoveItem()
	}
}

void UMDInventory::AddItem(UObject* Item)
{
	ItemTile->AddItem(Item);
}

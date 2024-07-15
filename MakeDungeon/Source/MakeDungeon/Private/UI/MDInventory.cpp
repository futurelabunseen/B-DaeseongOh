// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDInventory.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "Player/MDPlayerController.h"
#include "GameData/MDItemBaseData.h"

void UMDInventory::BuyItem(UMDItemBaseData* InItem)
{
	AMDPlayerController* MDPlayerController = Cast<AMDPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MDPlayerController)
	{
		UMDInventory* PlayerInventory = Cast<UMDInventory>(MDPlayerController->GetUI(EMDUIType::HUD_Inventory));
		if (PlayerInventory)
		{
			PlayerInventory->AddItem(InItem);
		}
	}
}

void UMDInventory::SellItem(UMDItemBaseData* InItem)
{
	UMDItemBaseData* SourceItem = Cast<UMDItemBaseData>(InItem);
	if (SourceItem)
	{
		ItemTile->RemoveItem(SourceItem);
	}
}

void UMDInventory::UpdateItemTile()
{
	for (int32 i = 0; i < 30; i++)
	{
		//ItemTile->AddItem()
		//ItemTile->RemoveItem()
	}
}

const TArray<UObject*> UMDInventory::GetAllItems()
{
	return ItemTile->GetListItems();
}

void UMDInventory::AddItem(UMDItemBaseData* InItem)
{
	UMDItemBaseData* NewItem = DuplicateObject<UMDItemBaseData>(InItem, this);

	if(NewItem)
	{
		ItemTile->AddItem(NewItem);
	}
}

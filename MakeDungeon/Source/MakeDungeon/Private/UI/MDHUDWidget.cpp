// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDHUDWidget.h"
#include "UI/MDStatusBarUserWidget.h"
#include "UI/MDCharacterStatWidget.h"
#include "UI/MDInventory_Item.h"
#include "UI/MDGameScoreWidget.h"
#include "UI/MDInventory.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Character/MDCharacterPlayer.h"
#include "GameData/MDItemBaseData.h"
#include "Components/Overlay.h"

void UMDHUDWidget::UpdateUI(EMDUIType UIType)
{
	UIs[UIType]->UpdateWidget();
}

void UMDHUDWidget::VisibleUI(EMDUIType UIType)
{
	if (ESlateVisibility::Visible == UIs[UIType]->GetVisibility())
	{
		UIs[UIType]->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UIs[UIType]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMDHUDWidget::HideUI(EMDUIType UIType)
{
	UIs[UIType]->SetVisibility(ESlateVisibility::Hidden);
}

void UMDHUDWidget::AddItem(UMDItemBaseData* InItem)
{
	UMDInventory* Inventory = Cast<UMDInventory>(UIs[EMDUIType::HUD_Inventory]);
	if(Inventory)
	{
		Inventory->AddItem(InItem);
	}
}

void UMDHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Guide = Cast<UOverlay>(GetWidgetFromName(TEXT("Guide")));
	ensure(Guide);
	Guide->SetVisibility(ESlateVisibility::Hidden);

	UIs.Add({ EMDUIType::HUD_StatusBar, Cast<UMDStatusBarUserWidget>(GetWidgetFromName(TEXT("WidgetStatusBar"))) });
	ensure(UIs[EMDUIType::HUD_StatusBar]);

	UIs.Add({ EMDUIType::HUD_Stat, Cast<UMDCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat"))) });
	ensure(UIs[EMDUIType::HUD_Stat]);

	UIs.Add({ EMDUIType::HUD_GameScore, Cast<UMDGameScoreWidget>(GetWidgetFromName(TEXT("WidgetGameScore"))) });
	ensure(UIs[EMDUIType::HUD_GameScore]);

	UIs.Add({ EMDUIType::HUD_Inventory, Cast<UMDInventory>(GetWidgetFromName(TEXT("WidgetInventory"))) });
	ensure(UIs[EMDUIType::HUD_Inventory]);
	UIs[EMDUIType::HUD_Inventory]->SetVisibility(ESlateVisibility::Hidden);

	AMDCharacterPlayer* MDPlayer = Cast<AMDCharacterPlayer>(GetOwningPlayerPawn());
	if (MDPlayer)
	{
		//MDPlayer->SetupHUDWidget(this);
		Cast<UMDUserWidget>(UIs[EMDUIType::HUD_StatusBar])->SetAbilitySystemComponent(MDPlayer);
		Cast<UMDUserWidget>(UIs[EMDUIType::HUD_Stat])->SetAbilitySystemComponent(MDPlayer);
	}
}

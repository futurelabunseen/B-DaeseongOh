// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterVillager.h"
#include "Components/BoxComponent.h"
#include "Physics/MDCollision.h"
#include "Player/MDPlayerController.h"
#include "UI/MDInventory.h"
#include "GameData/MDItemBaseData.h"
#include "../MakeDungeon.h"

AMDCharacterVillager::AMDCharacterVillager()
{
	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetupAttachment(GetMesh());
	InteractionTrigger->SetCollisionProfileName(CPROFILE_MDTRIGGER);
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, 
								&AMDCharacterVillager::OnInteractionTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this,
								&AMDCharacterVillager::OnInteractionTriggerEndOverlap);
}

void AMDCharacterVillager::BeginPlay()
{
	Super::BeginPlay();

	MDShopWidget = CreateWidget<UMDInventory>(GetWorld()->GetFirstPlayerController(), MDShopWidgetClass);

	for (auto Item : ShopItems)
	{
		MDShopWidget->AddItem(Item);
	}
}

void AMDCharacterVillager::VisibleShop()
{
	AMDPlayerController* PlayerController = Cast<AMDPlayerController>(GetWorld()->GetFirstPlayerController());

	if (!MDShopWidget->IsInViewport())
	{
		MDShopWidget->AddToPlayerScreen();
		PlayerController->VisibleUI(EMDUIType::HUD_Inventory);
	}
	else
	{
		if (ESlateVisibility::Hidden != MDShopWidget->GetVisibility())
		{
			MDShopWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerController->HideUI(EMDUIType::HUD_Inventory);
		}
		else
		{
			MDShopWidget->SetVisibility(ESlateVisibility::Visible);
			PlayerController->VisibleUI(EMDUIType::HUD_Inventory);
		}
	}
}

void AMDCharacterVillager::OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMDPlayerController* PlayerController = Cast<AMDPlayerController>(OtherActor->GetInstigatorController());

	if(PlayerController)
	{
		PlayerController->OnInteraction.AddDynamic(this, &AMDCharacterVillager::VisibleShop);
	}
}

void AMDCharacterVillager::OnInteractionTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMDPlayerController* PlayerController = Cast<AMDPlayerController>(OtherActor->GetInstigatorController());

	if (PlayerController) 
	{
		PlayerController->OnInteraction.Clear();
		PlayerController->HideUI(EMDUIType::HUD_Inventory);
		MDShopWidget->RemoveFromParent();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDHUDWidget.h"
#include "UI/MDHpBarUserWidget.h"
#include "UI/MDMpBarUserWidget.h"
#include "UI/MDCharacterStatWidget.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Character/MDCharacterPlayer.h"

UMDHUDWidget::UMDHUDWidget(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
}

void UMDHUDWidget::UpdateHpBar(const FOnAttributeChangeData& ChangeData)
{
	HpBar->UpdateHpBar();
}

void UMDHUDWidget::UpdateMpBar(const FOnAttributeChangeData& ChangeData)
{
	MpBar->UpdateMpBar();
}

void UMDHUDWidget::UpdateStat(const FOnAttributeChangeData& ChangeData)
{
	CharacterStat->UpdateStat();
}

void UMDHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UMDHpBarUserWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	MpBar = Cast<UMDMpBarUserWidget>(GetWidgetFromName(TEXT("WidgetMpBar")));
	ensure(MpBar);

	CharacterStat = Cast<UMDCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	AMDCharacterPlayer* MDPlayer = Cast<AMDCharacterPlayer>(GetOwningPlayerPawn());
	if (MDPlayer)
	{
		//MDPlayer->SetupHUDWidget(this);
		HpBar->SetAbilitySystemComponent(MDPlayer);
		MpBar->SetAbilitySystemComponent(MDPlayer);
		CharacterStat->SetAbilitySystemComponent(MDPlayer);
	}
}

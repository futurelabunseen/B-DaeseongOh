// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDMpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMDMpBarUserWidget::UpdateMpBar()
{
	if (PbMpBar)
	{
		PbMpBar->SetPercent(CurrentMana / CurrentMaxMana);
	}

	if (TxtMpStat)
	{
		TxtMpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentMana, CurrentMaxMana)));
	}
}

void UMDMpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetManaAttribute()).AddUObject(this, &UMDMpBarUserWidget::OnManaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UMDMpBarUserWidget::OnMaxManaChanged);

		PbMpBar->SetFillColorAndOpacity(ManaColor);

		const UMDCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UMDCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentMana = CurrentAttributeSet->GetHealth();
			CurrentMaxMana = CurrentAttributeSet->GetMaxHealth();
			UpdateMpBar();
		}

		if (CurrentMaxMana > 0.f)
		{
			UpdateMpBar();
		}
	}
}

void UMDMpBarUserWidget::OnManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateMpBar();
}

void UMDMpBarUserWidget::OnMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateMpBar();
}

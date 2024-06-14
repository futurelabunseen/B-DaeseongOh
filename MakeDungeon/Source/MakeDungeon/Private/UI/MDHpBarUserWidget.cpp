// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tags/MDGameplayTag.h"

void UMDHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}

	if (PbMpBar)
	{
		PbMpBar->SetPercent(CurrentMana / CurrentMaxMana);
	}
	
	if (TxtMpStat)
	{
		TxtMpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentMana, CurrentMaxMana)));
	}
}

void UMDHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UMDHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UMDHpBarUserWidget::OnMaxHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetManaAttribute()).AddUObject(this, &UMDHpBarUserWidget::OnManaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UMDHpBarUserWidget::OnMaxManaChanged);
		ASC->RegisterGameplayTagEvent(MDTAG_CHARACTER_INVINCIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMDHpBarUserWidget::OnInvinsibleTagChanged);

		PbHpBar->SetFillColorAndOpacity(HealthColor);
		PbMpBar->SetFillColorAndOpacity(ManaColor);

		const UMDCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UMDCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			CurrentMana = CurrentAttributeSet->GetMana();
			CurrentMaxMana = CurrentAttributeSet->GetMaxMana();
			UpdateHpBar();
		}

		if (CurrentMaxHealth > 0.f)
		{
			UpdateHpBar();
		}

		if (CurrentMaxMana > 0.f)
		{
			UpdateHpBar();
		}
	}
}

void UMDHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UMDHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UMDHpBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvincibleColor);
		PbHpBar->SetPercent(1.f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

void UMDHpBarUserWidget::OnManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateHpBar();
}

void UMDHpBarUserWidget::OnMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateHpBar();
}

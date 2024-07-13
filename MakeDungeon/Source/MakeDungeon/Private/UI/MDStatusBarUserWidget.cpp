// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDStatusBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tags/MDGameplayTag.h"

void UMDStatusBarUserWidget::UpdateWidget()
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

void UMDStatusBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UMDStatusBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UMDStatusBarUserWidget::OnMaxHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetManaAttribute()).AddUObject(this, &UMDStatusBarUserWidget::OnManaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UMDStatusBarUserWidget::OnMaxManaChanged);
		ASC->RegisterGameplayTagEvent(MDTAG_CHARACTER_INVINCIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMDStatusBarUserWidget::OnInvinsibleTagChanged);

		PbHpBar->SetFillColorAndOpacity(HealthColor);
		PbMpBar->SetFillColorAndOpacity(ManaColor);

		const UMDCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UMDCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			CurrentMana = CurrentAttributeSet->GetMana();
			CurrentMaxMana = CurrentAttributeSet->GetMaxMana();
			UpdateWidget();
		}

		if (CurrentMaxHealth > 0.f)
		{
			UpdateWidget();
		}

		if (CurrentMaxMana > 0.f)
		{
			UpdateWidget();
		}
	}
}

void UMDStatusBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateWidget();
}

void UMDStatusBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateWidget();
}

void UMDStatusBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvincibleColor);
		PbHpBar->SetPercent(1.f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateWidget();
	}
}

void UMDStatusBarUserWidget::OnManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateWidget();
}

void UMDStatusBarUserWidget::OnMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateWidget();
}

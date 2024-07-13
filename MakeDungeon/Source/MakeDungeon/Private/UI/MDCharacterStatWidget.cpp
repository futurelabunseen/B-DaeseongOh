// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDCharacterStatWidget.h"
#include "Character/MDCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMDCharacterStatWidget::UpdateWidget()
{
	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, CurrentMaxHealth)));
	}

	if (TxtMpStat)
	{
		TxtMpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMana, CurrentMaxMana)));
	}

	if (TxtCurrentWeapon)
	{
		FGameplayTag CurrentWeapon = Cast<AMDCharacterPlayer>(GetOwningPlayerPawn())->GetWeaponType();
		TxtCurrentWeapon->SetText(FText::FromString(CurrentWeapon.ToString()));
	}
}

void UMDCharacterStatWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	AMDCharacterPlayer* MDPlayer = Cast<AMDCharacterPlayer>(InOwner);

	if (ASC && MDPlayer)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UMDCharacterStatWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UMDCharacterStatWidget::OnMaxHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetManaAttribute()).AddUObject(this, &UMDCharacterStatWidget::OnManaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UMDCharacterAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UMDCharacterStatWidget::OnMaxManaChanged);

		MDPlayer->OnGameplayTagChanged.AddUObject(this, &UMDCharacterStatWidget::UpdateWidget);

		const UMDCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UMDCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			CurrentMana = CurrentAttributeSet->GetHealth();
			CurrentMaxMana = CurrentAttributeSet->GetMaxHealth();

			UpdateWidget();
		}
	}
}

void UMDCharacterStatWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateWidget();
}

void UMDCharacterStatWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateWidget();
}

void UMDCharacterStatWidget::OnManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateWidget();
}

void UMDCharacterStatWidget::OnMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateWidget();
}

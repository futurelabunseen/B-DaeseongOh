// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidget.h"
#include "GameplayEffectTypes.h"
#include "MDStatusBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDStatusBarUserWidget : public UMDUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void UpdateWidget() override;

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
protected:

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void OnManaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxManaChanged(const FOnAttributeChangeData& ChangeData);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbMpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtMpStat;

	float CurrentHealth = 0.f;
	float CurrentMaxHealth = 0.1f;
	float CurrentMana= 0.f;
	float CurrentMaxMana = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvincibleColor = FLinearColor::Green;
	FLinearColor ManaColor = FLinearColor::Blue;
};

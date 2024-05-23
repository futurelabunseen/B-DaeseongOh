// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidget.h"
#include "GameplayEffectTypes.h"
#include "MDCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDCharacterStatWidget : public UMDUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateStat();

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
protected:

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnManaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxManaChanged(const FOnAttributeChangeData& ChangeData);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtMpStat;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtCurrentWeapon;

	float CurrentHealth = 0.f;
	float CurrentMaxHealth = 0.f;

	float CurrentMana = 0.f;	
	float CurrentMaxMana = 0.f;
};

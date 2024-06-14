// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidget.h"
#include "GameplayEffectTypes.h"
#include "MDMpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDMpBarUserWidget : public UMDUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateMpBar();

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
protected:

	virtual void OnManaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxManaChanged(const FOnAttributeChangeData& ChangeData);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbMpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtMpStat;

	float CurrentMana = 0.f;
	float CurrentMaxMana = 0.1f;

	FLinearColor ManaColor = FLinearColor::Blue;
};

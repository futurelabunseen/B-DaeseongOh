// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "MDHUDWidget.generated.h"

class UMDHpBarUserWidget;
class UMDMpBarUserWidget;
class UMDCharacterStatWidget;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMDHUDWidget(const FObjectInitializer& ObjectInitializer);

	UMDCharacterStatWidget* GetStatWidget() { return CharacterStat; }
	
	void UpdateHpBar(const FOnAttributeChangeData& ChangeData);
	void UpdateMpBar(const FOnAttributeChangeData& ChangeData);
	void UpdateStat(const FOnAttributeChangeData& ChangeData);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UMDHpBarUserWidget> HpBar;

	UPROPERTY()
	TObjectPtr<UMDMpBarUserWidget> MpBar;

	UPROPERTY()
	TObjectPtr<UMDCharacterStatWidget> CharacterStat;
};

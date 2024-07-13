// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidgetBase.h"
#include "GameplayEffectTypes.h"
#include "UI/MDUIType.h"
#include "MDHUDWidget.generated.h"

class UMDItemBaseData;
class UOverlay;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDHUDWidget : public UMDUserWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UMDUserWidgetBase* GetUI(EMDUIType UIType) { return UIs[UIType]; }

	void UpdateUI(EMDUIType UIType);

	void VisibleUI(EMDUIType UIType);
	void HideUI(EMDUIType UIType);
	void AddItem(EMDUIType UIType);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TMap<EMDUIType, TObjectPtr<UMDUserWidgetBase>> UIs;

	UPROPERTY()
	TObjectPtr<UOverlay> Guide;

	//ForDebugTest
	UPROPERTY(EditAnywhere, Category = "Debug")
	TObjectPtr<UMDItemBaseData> ItemData;
};

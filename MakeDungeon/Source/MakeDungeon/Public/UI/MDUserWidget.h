// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MDUserWidgetBase.h"
#include "AbilitySystemInterface.h"
#include "MDUserWidget.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDUserWidget : public UMDUserWidgetBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;
};

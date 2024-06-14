// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UMDUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UMDUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}

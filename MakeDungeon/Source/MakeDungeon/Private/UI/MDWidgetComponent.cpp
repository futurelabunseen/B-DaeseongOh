// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDWidgetComponent.h"
#include "UI/MDUserWidget.h"

void UMDWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMDUserWidget* MDUserWidget = Cast<UMDUserWidget>(GetWidget());
	if (MDUserWidget)
	{
		MDUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MDPierceCountWidget.h"
#include "Components/TextBlock.h"

void UMDPierceCountWidget::SetCount(int32 NewCount)
{
	Count = NewCount;
	TxtCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDGameScoreWidget.h"
#include "Components/TextBlock.h"


void UMDGameScoreWidget::UpdateWidget()
{
	if (TxtRemainingTarget)
	{
		TxtRemainingTarget->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainingTarget)));
	}

	if (TxtRemainingSpawnTime)
	{
		TxtRemainingSpawnTime->SetText(FText::FromString(FString::Printf(TEXT("%d Sec."), RemainingSpawnTime)));
	}
}

void UMDGameScoreWidget::SetScore(int32 NewScore)
{
	RemainingTarget = NewScore;
	UpdateWidget();
}

void UMDGameScoreWidget::SetSpawnTime(int32 NewTime)
{
	RemainingSpawnTime = NewTime;
	UpdateWidget();
}

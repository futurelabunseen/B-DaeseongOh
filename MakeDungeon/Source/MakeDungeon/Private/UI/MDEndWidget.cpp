// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDEndWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameMode.h"

void UMDEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtnRestart)
	{
		BtnRestart->OnClicked.AddDynamic(this, &UMDEndWidget::RestartButtonCallback);
	}
	if (BtnEnd)
	{
		BtnEnd->OnClicked.AddDynamic(this, &UMDEndWidget::EndButtonCallback);
	}
}


void UMDEndWidget::RestartButtonCallback()
{
	AGameMode* GM = Cast<AGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RestartGame();
	}
}

void UMDEndWidget::EndButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->
		GetFirstPlayerController(), EQuitPreference::Quit, true);
}

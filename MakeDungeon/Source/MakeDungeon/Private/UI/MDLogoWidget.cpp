// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MDLogoWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMDLogoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(BtnStart)
	{
		BtnStart->OnClicked.AddDynamic(this, &UMDLogoWidget::StartButtonCallback);
	}
	if(BtnEnd)
	{
		BtnEnd->OnClicked.AddDynamic(this, &UMDLogoWidget::EndButtonCallback);
	}
}

void UMDLogoWidget::StartButtonCallback()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Village"));
}

void UMDLogoWidget::EndButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->
		GetFirstPlayerController(), EQuitPreference::Quit, true);
}

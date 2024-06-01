// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MDLogoWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDLogoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
	void StartButtonCallback();

	UFUNCTION(BlueprintCallable)
	void EndButtonCallback();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> BtnStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> BtnEnd;
};

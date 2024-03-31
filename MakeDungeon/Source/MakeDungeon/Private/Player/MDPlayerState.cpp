// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"

AMDPlayerState::AMDPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AMDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

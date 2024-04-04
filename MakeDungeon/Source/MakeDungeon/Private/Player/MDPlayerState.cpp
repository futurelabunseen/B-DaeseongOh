// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"

AMDPlayerState::AMDPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UMDCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AMDPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UMDCharacterAttributeSet* AMDPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"

AMDCharacterNonPlayer::AMDCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UMDCharacterAttributeSet>(TEXT("AttributeSet"));
}

void AMDCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}

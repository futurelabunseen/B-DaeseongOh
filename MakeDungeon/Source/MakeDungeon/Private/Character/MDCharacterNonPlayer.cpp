// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "AI/MDAIController.h"

AMDCharacterNonPlayer::AMDCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UMDCharacterAttributeSet>(TEXT("AttributeSet"));

	AIControllerClass = AMDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Level = 1;
}

void AMDCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	//AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AMDCharacterNonPlayer::OnOutOfHealth()
{
	Super::OnOutOfHealth();
}

float AMDCharacterNonPlayer::GetAIPatrolRadius()
{
	return 500.0f;
}

float AMDCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AMDCharacterNonPlayer::GetAIAttackRange()
{
	return 0.0f;
}

float AMDCharacterNonPlayer::GetAITurnSpeed()
{
	return 0.0f;
}

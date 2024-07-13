// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Effects/MDSynergyExecutionCalc.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "AbilitySystemComponent.h"

void UMDSynergyExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(SourceASC->GetOwner());

		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(TempEffect, 0, EffectContextHandle);

		SourceASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UMDCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, 0.f));
		//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UMDCharacterAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, Mana));
	}
}

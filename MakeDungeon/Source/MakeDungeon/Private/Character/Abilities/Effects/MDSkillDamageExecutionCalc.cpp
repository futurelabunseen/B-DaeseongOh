// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Effects/MDSkillDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Character/Abilities/AttributeSets/MDCharacterSkillAttributeSet.h"

void UMDSkillDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceASC && TargetASC)
		{
			const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UMDCharacterSkillAttributeSet::GetSkillRangeAttribute());
			const float MaxDamage = SourceASC->GetNumericAttributeBase(UMDCharacterSkillAttributeSet::GetSkillAttackRateAttribute());
			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.f, MaxDamageRange);
			const float InvDamageRatio = 1.f - Distance / MaxDamageRange;
			float Damage = InvDamageRatio * MaxDamage;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UMDCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}

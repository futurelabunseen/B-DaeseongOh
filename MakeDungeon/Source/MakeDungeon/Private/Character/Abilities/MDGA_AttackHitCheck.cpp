// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AttackHitCheck.h"
#include "Character/Abilities/Tasks/MDAT_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "../MakeDungeon.h"

UMDGA_AttackHitCheck::UMDGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UMDAT_Trace* AttackTraceTask = UMDAT_Trace::CreateTask(this, AMDTA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UMDGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		MD_LOG(LogMD, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		if (!SourceASC || !TargetASC)
		{
			MD_LOG(LogMD, Log, TEXT("ASC not fount"));
			return;
		}

		const UMDCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UMDCharacterAttributeSet>();
		UMDCharacterAttributeSet* TargetAttribute = const_cast<UMDCharacterAttributeSet*>(TargetASC->GetSet<UMDCharacterAttributeSet>());
		if (!SourceAttribute || !TargetAttribute)
		{
			MD_LOG(LogMD, Log, TEXT("Attribute not fount"));
			return;
		}

		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

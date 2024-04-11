// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AttackHitCheck.h"
#include "Character/Abilities/Tasks/MDAT_Trace.h"
#include "Character/Abilities/Tasks/MDAT_Projectile.h"
#include "Character/Abilities/TargetActors/MDTA_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Projectile.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../MakeDungeon.h"
#include "Tags/MDGameplayTag.h"

UMDGA_AttackHitCheck::UMDGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;

	if (MDTAG_HITCHECK_TRACE == TriggerEventData->EventTag)
	{
		UMDAT_Trace* AttackTraceTask = UMDAT_Trace::CreateTask(this, AMDTA_Trace::StaticClass());
		AttackTraceTask->OnComplete.AddDynamic(this, &UMDGA_AttackHitCheck::OnTraceResultCallback);
		AttackTraceTask->ReadyForActivation();
		MD_LOG(LogMD, Log, TEXT("Activate"));
	}
	else if (MDTAG_HITCHECK_PROJECTILE == TriggerEventData->EventTag)
	{
		/*UMDAT_Projectile* AttackProjectileTask = UMDAT_Projectile::CreateTask(this, AMDTA_Projectile::StaticClass());
		AttackProjectileTask->OnComplete.AddDynamic(this, &UMDGA_AttackHitCheck::OnTraceResultCallback);
		AttackProjectileTask->ReadyForActivation();
		MD_LOG(LogMD, Log, TEXT("Activate"));*/
	}
	else
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
	}
	
}

void UMDGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		MD_LOG(LogMD, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		const UMDCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UMDCharacterAttributeSet>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(MDTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

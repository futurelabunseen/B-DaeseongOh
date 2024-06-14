// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AttackHitCheck.h"
#include "Character/Abilities/Tasks/MDAT_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Projectile.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../MakeDungeon.h"
#include "Tags/MDGameplayTag.h"

#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UMDGA_AttackHitCheck::UMDGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;
	SpawnLocation = FVector::ZeroVector;

	if (TriggerEventData->ContextHandle.HasOrigin())
	{
		SpawnLocation = TriggerEventData->ContextHandle.GetOrigin();
	}

	UMDAT_Trace* AttackTraceTask = UMDAT_Trace::CreateTask(this, TargetActorClass, SpawnLocation);
	AttackTraceTask->OnComplete.AddDynamic(this, &UMDGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if(NiagaraFX)
	{
		ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
		if(Character)
		{
			FVector Origin;
			if (!SpawnLocation.IsZero())
			{
				Origin = SpawnLocation;
			}
			else
			{
				Origin = Character->GetMesh()->GetSocketLocation(FName("weapon_r"));
			}
			Origin.Z += 45.0;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraFX, Origin, FRotator::ZeroRotator, FVector(VFXScale));
		}
	}

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		MD_LOG(LogMD, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			MD_LOG(LogMD, Error, TEXT("ASC not found!"));
			return;
		}

		const UMDCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UMDCharacterAttributeSet>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			TargetASC->ExecuteGameplayCue(HitEffectTag, CueParam);
		}

		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
		if (BuffEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}

		for (auto TargetDebuffEffect : TargetDebuffEffects)
		{
			FGameplayEffectSpecHandle DebuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TargetDebuffEffect);
			if (DebuffEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DebuffEffectSpecHandle, TargetDataHandle);
			}
		}
	}
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		//UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(HitEffectTag, CueParam);
		}

		for (auto TargetDebuffEffect : TargetDebuffEffects)
		{
			FGameplayEffectSpecHandle DebuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TargetDebuffEffect);
			if (DebuffEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DebuffEffectSpecHandle, TargetDataHandle);
			}
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

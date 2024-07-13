// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AttackHitCheck.h"
#include "Character/Abilities/Tasks/MDAT_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Projectile.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "Character/MDCharacterBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../MakeDungeon.h"
#include "Tags/MDGameplayTag.h"
#include "Character/Abilities/AttributeSets/MDCharacterSkillAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UMDGA_AttackHitCheck::UMDGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ImpulsePower = 1000.0;
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

	if (nullptr != TriggerEventData->OptionalObject)
	{
		NiagaraFX = Cast<UNiagaraSystem>(TriggerEventData->OptionalObject);
	}

	UMDAT_Trace* AttackTraceTask = UMDAT_Trace::CreateTask(this, TargetActorClass, SpawnLocation);
	AttackTraceTask->OnComplete.AddDynamic(this, &UMDGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	AMDCharacterBase* SourceMDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	if(NiagaraFX)
	{
		if(SourceMDCharacter)
		{
			FVector Origin;
			if (!SpawnLocation.IsZero())
			{
				Origin = SpawnLocation;
			}
			else
			{
				Origin = SourceMDCharacter->GetMesh()->GetSocketLocation(FName("weapon_r"));
			}
			//Origin.Z += 45.0;

			if(ApplyScale)
			{
				const UMDCharacterSkillAttributeSet* MDChSKAtt = GetAbilitySystemComponentFromActorInfo()->GetSet<UMDCharacterSkillAttributeSet>();
				VFXScale = MDChSKAtt->GetSkillRange() / 150.f;
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraFX, Origin, FRotator::ZeroRotator, FVector(VFXScale));
			}
			else
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraFX, Origin, FRotator::ZeroRotator, FVector(VFXScale));
			}
		}
	}

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		MD_LOG(LogMD, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
		AMDCharacterBase* TargetMDCharacter = Cast<AMDCharacterBase>(HitResult.GetActor());

		if (SourceMDCharacter && TargetMDCharacter)
		{
			const FVector Direction = SourceMDCharacter->GetAttackDirection().Vector() * ImpulsePower;
			TargetMDCharacter->GetCharacterMovement()->AddImpulse(Direction, true);
		}

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

		for (const auto& SourceBuffEffect : SourceBuffEffects)
		{
			FGameplayEffectSpecHandle SourceBuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SourceBuffEffectSpecHandle);
		}

		for (const auto& TargetDebuffEffect : TargetDebuffEffects)
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

		for (const auto& HitActor : TargetDataHandle.Data[0].Get()->GetActors())
		{
			AMDCharacterBase* TargetMDCharacter = Cast<AMDCharacterBase>(HitActor.Get());

			if (SourceMDCharacter && TargetMDCharacter)
			{
				const FVector Direction = SourceMDCharacter->GetAttackDirection().Vector() * ImpulsePower;
				TargetMDCharacter->GetCharacterMovement()->AddImpulse(Direction, true);
			}
		}

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

		for (const auto& SourceBuffEffect : SourceBuffEffects)
		{
			FGameplayEffectSpecHandle SourceBuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SourceBuffEffectSpecHandle);
		}

		for (const auto& TargetDebuffEffect : TargetDebuffEffects)
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

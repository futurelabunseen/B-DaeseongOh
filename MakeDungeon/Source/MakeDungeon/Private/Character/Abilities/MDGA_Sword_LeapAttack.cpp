// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Sword_LeapAttack.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_Sword_LeapAttack::UMDGA_Sword_LeapAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Sword_LeapAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	
	if (Montage)
	{
		MDCharacter->StopMovement();
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Montage);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Sword_LeapAttack::OnCompletedCallback);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Sword_LeapAttack::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();
	}

	// For Effect
	FGameplayEffectSpecHandle SkillInitEffectHandle = MakeOutgoingGameplayEffectSpec(SkillInitEffect);
	if (SkillInitEffectHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SkillInitEffectHandle);
	}
}

void UMDGA_Sword_LeapAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(MDTAG_EVENT_CHARACTER_TRACESKILL.GetSingleTagContainer());

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Sword_LeapAttack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_Sword_LeapAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

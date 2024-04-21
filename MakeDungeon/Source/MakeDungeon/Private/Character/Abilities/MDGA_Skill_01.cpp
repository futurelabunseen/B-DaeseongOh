// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Skill_01.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Item/MDWeaponBase.h"
#include "../MakeDungeon.h"

UMDGA_Skill_01::UMDGA_Skill_01()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Skill_01::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	UMDWeaponBase* Weapon = MDCharacter->GetWeapon();
	if (!Weapon->GetWeaponAttackData())
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
		MD_LOG(LogMD, Log, TEXT("No Weapon"));
		return;
	}

	CurrentWeaponAttackData = Weapon->GetWeaponAttackData();
	if (CurrentWeaponAttackData->SkillMontage.IsEmpty())
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
		MD_LOG(LogMD, Log, TEXT("SkillMontageIsEmpty"));
		return;
	}

	UAnimMontage* SkillMontage = Weapon->GetSkillMontage(EMDSkillMontage::Skill_01);
	if (SkillMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlaySkill_01"), SkillMontage);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Skill_01::OnCompletedCallback);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Skill_01::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();

		MDCharacter->StopMovement();
	}
	else
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
		MD_LOG(LogMD, Log, TEXT("No SkillMontage"));
		return;
	}

	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_Skill_01::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
}

void UMDGA_Skill_01::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMDGA_Skill_01::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Skill_01::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_Skill_01::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

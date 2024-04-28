// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Skill_01.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Item/MDWeaponBase.h"
#include "Animation/MDAnimInstance.h"
#include "../MakeDungeon.h"

UMDGA_Skill_01::UMDGA_Skill_01()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AnimPlaySpeed = 0.5f;
}

void UMDGA_Skill_01::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	
	bInputReleased = false;
	MDCharacter->SetIsCharged(true);
	UMDAnimInstance* AnimInst = Cast<UMDAnimInstance>(ActorInfo->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetAnimPlaySpeed(AnimPlaySpeed);
	}

	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_Skill_01::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
}

void UMDGA_Skill_01::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(!bInputReleased)
	{
		bInputReleased = true;

		AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
		MDCharacter->SetIsCharged(false);
		
		/*MDCharacter->StopAnimMontage();

		UMDWeaponBase* Weapon = MDCharacter->GetWeapon();

		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, 
			TEXT("PlaySkill_01"), Weapon->GetSkillMontage(EMDSkillMontage::Skill_01), 1.f, FName("SkillEnd"));
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Skill_01::OnCompletedEnd);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Skill_01::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();*/
		MD_LOG(LogMD, Log, TEXT("Released"));

		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
	}
}

void UMDGA_Skill_01::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMDGA_Skill_01::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Skill_01::OnCompletedStart()
{
	MD_LOG(LogMD, Log, TEXT("Charged"));
}

void UMDGA_Skill_01::OnCompletedEnd()
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(CurrentActorInfo->AvatarActor.Get());
	MDCharacter->SetIsCharged(false);
	bInputReleased = false;

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
	MD_LOG(LogMD, Log, TEXT("End"));
}

void UMDGA_Skill_01::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_PlayAnim_Once.h"
#include "Character/MDCharacterBase.h"
#include "Item/MDWeaponBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../MakeDungeon.h"

UMDGA_PlayAnim_Once::UMDGA_PlayAnim_Once()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_PlayAnim_Once::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	UMDWeaponBase* Weapon = MDCharacter->GetWeapon();

	UAnimMontage* SkillMontage = Weapon->GetMontage(EMDAttackType::Skill_02);
	if (SkillMontage)
	{
		MDCharacter->StopMovement();
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), SkillMontage);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_PlayAnim_Once::OnCompleted);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_PlayAnim_Once::OnInterrupted);
		PlayAttackMontageTask->ReadyForActivation();
	}
}

void UMDGA_PlayAnim_Once::OnCompleted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_PlayAnim_Once::OnInterrupted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_PlayAnim_Charge.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Item/MDWeaponBase.h"
#include "Animation/MDAnimInstance.h"
#include "../MakeDungeon.h"

UMDGA_PlayAnim_Charge::UMDGA_PlayAnim_Charge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AnimPlaySpeed = 0.5f;
}

void UMDGA_PlayAnim_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

void UMDGA_PlayAnim_Charge::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!bInputReleased)
	{
		bInputReleased = true;
		OnCompleted();
	}

	MD_LOG(LogMD, Log, TEXT("Released"));
}

void UMDGA_PlayAnim_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(CurrentActorInfo->AvatarActor.Get());
	MDCharacter->SetIsCharged(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_PlayAnim_Charge::OnCompleted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_PlayAnim_Charge::OnInterrupted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

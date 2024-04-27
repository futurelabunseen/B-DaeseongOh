// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_ArrowRain.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirmCancel.h"

UMDGA_Bow_ArrowRain::UMDGA_Bow_ArrowRain()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Bow_ArrowRain::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	FVector TargetLocation = MDCharacter->GetAttackLocation();

	/*UAbilityTask_WaitConfirmCancel* ConfirmCancelTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this);
	ConfirmCancelTask->OnConfirm.AddDynamic(this, &UMDGA_Bow_ArrowRain::OnConfirm);
	ConfirmCancelTask->OnCancel.AddDynamic(this, &UMDGA_Bow_ArrowRain::OnCancel);
	ConfirmCancelTask->ReadyForActivation();*/
}

void UMDGA_Bow_ArrowRain::OnConfirm()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMDGA_Bow_ArrowRain::OnCancel()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Sword_LeapAttack.h"
#include "Character/MDCharacterBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Item/MDWeaponBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../MakeDungeon.h"

UMDGA_Sword_LeapAttack::UMDGA_Sword_LeapAttack()
{
}

void UMDGA_Sword_LeapAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	UAnimMontage* SkillMontage = Weapon->GetSkillMontage();
	if (SkillMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), SkillMontage);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Sword_LeapAttack::OnCompletedCallback);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Sword_LeapAttack::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();
	}

	MDCharacter->SetIsTrackingTarget(true);
}

void UMDGA_Sword_LeapAttack::OnCompletedCallback()
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(CurrentActorInfo->AvatarActor.Get());
	MDCharacter->SetIsTrackingTarget(false);

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
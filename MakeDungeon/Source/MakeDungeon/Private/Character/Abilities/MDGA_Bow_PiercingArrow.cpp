// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_PiercingArrow.h"
#include "Character/MDCharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_Bow_PiercingArrow::UMDGA_Bow_PiercingArrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Bow_PiercingArrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterPlayer* MDCharacter = CastChecked<AMDCharacterPlayer>(ActorInfo->AvatarActor.Get());
	MDCharacter->SetVisiblePierceCount(true);
	MDCharacter->SetPierceCount(3);

	if (Montage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Montage);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Bow_PiercingArrow::OnCompleted);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Bow_PiercingArrow::OnInterrupted);
		PlayAttackMontageTask->ReadyForActivation();
	}
	MDCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	MDCharacter->SetIsTrackingTarget(true);

	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UMDGA_Bow_PiercingArrow::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	MD_LOG(LogMD, Log, TEXT("Pressed %d"), AttackCount);

	AMDCharacterPlayer* MDCharacter = CastChecked<AMDCharacterPlayer>(ActorInfo->AvatarActor.Get());
}

void UMDGA_Bow_PiercingArrow::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterPlayer* MDCharacter = CastChecked<AMDCharacterPlayer>(ActorInfo->AvatarActor.Get());

	++AttackCount;
	if (3 <= AttackCount)
	{
		MDCharacter->SetIsTrackingTarget(false);
		MDCharacter->SetVisiblePierceCount(false);
	}
	MDCharacter->SetPierceCount(3 - AttackCount);
	NextSection();
}

void UMDGA_Bow_PiercingArrow::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	
}

void UMDGA_Bow_PiercingArrow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AttackCount = 0;
	AMDCharacterPlayer* MDCharacter = CastChecked<AMDCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (MDCharacter)
	{
		MDCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Bow_PiercingArrow::OnCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMDGA_Bow_PiercingArrow::OnInterrupted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_Bow_PiercingArrow::NextSection()
{
	FName NextSection = *FString::Printf(TEXT("Fire%d"), AttackCount);
	MontageJumpToSection(NextSection);
}

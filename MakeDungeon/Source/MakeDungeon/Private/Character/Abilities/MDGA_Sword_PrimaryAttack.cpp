// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Sword_PrimaryAttack.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/MDComboAttackData.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"


UMDGA_Sword_PrimaryAttack::UMDGA_Sword_PrimaryAttack()
{
}

void UMDGA_Sword_PrimaryAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	if (Montage && CurrentComboAttackData)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Montage, 1.f, GetNextSection());
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Sword_PrimaryAttack::OnCompleted);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Sword_PrimaryAttack::OnInterrupted);
		PlayAttackMontageTask->ReadyForActivation();

		StartComboTimer();

		MDCharacter->StopMovement();
	}

	MDCharacter->SetIsTrackingTarget(true);
}

void UMDGA_Sword_PrimaryAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (HasNextAttackInput && CurrentCombo < CurrentComboAttackData->MaxComboCount)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextAttackInput = false;
		//MD_LOG(LogMD, Log, TEXT("NextAnim"));
	}
}

void UMDGA_Sword_PrimaryAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CurrentComboAttackData = nullptr;
	CurrentCombo = 0;
	HasNextAttackInput = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Sword_PrimaryAttack::OnCompleted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_Sword_PrimaryAttack::OnInterrupted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UMDGA_Sword_PrimaryAttack::GetNextSection()
{
	FName NextSection;
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboAttackData->MaxComboCount);
	NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboAttackData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UMDGA_Sword_PrimaryAttack::StartComboTimer()
{
	if (ComboTimerHandle.IsValid())
	{
		ComboTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMDGA_Sword_PrimaryAttack::CheckComboInput, CurrentComboAttackData->InputThreshold, false);
}

void UMDGA_Sword_PrimaryAttack::CheckComboInput()
{
	HasNextAttackInput = true;
}

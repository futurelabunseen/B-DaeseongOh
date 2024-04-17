// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_PlayMontage.h"
#include "Character/MDCharacterBase.h"
#include "Item/MDWeaponBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../MakeDungeon.h"

UMDGA_PlayMontage::UMDGA_PlayMontage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_PlayMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	UAnimMontage* AttackMontage = Weapon->GetAttackMontage();
	if (AttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.f, GetNextSection());
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_PlayMontage::OnCompletedCallback);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_PlayMontage::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();

		StartComboTimer();
	}

	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_PlayMontage::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	MD_LOG(LogMD, Log, TEXT("InputPressed"));
	if (HasNextAttackInput && CurrentCombo < CurrentWeaponAttackData->MaxComboCount)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextAttackInput = false;
		MD_LOG(LogMD, Log, TEXT("NextAnim"));
	}
}

void UMDGA_PlayMontage::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMDGA_PlayMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentWeaponAttackData = nullptr;
	CurrentCombo = 0;
	HasNextAttackInput = false;
}

void UMDGA_PlayMontage::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_PlayMontage::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UMDGA_PlayMontage::GetNextSection(bool bIsSecondary)
{
	FName NextSection;
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentWeaponAttackData->MaxComboCount);
	NextSection = *FString::Printf(TEXT("%s%d"), *CurrentWeaponAttackData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UMDGA_PlayMontage::StartComboTimer()
{
	if (ComboTimerHandle.IsValid())
	{
		ComboTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMDGA_PlayMontage::CheckComboInput, CurrentWeaponAttackData->InputThreshold, false);
}

void UMDGA_PlayMontage::CheckComboInput()
{
	HasNextAttackInput = true;
}

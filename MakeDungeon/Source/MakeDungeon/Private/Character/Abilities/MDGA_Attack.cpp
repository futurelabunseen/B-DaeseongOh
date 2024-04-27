// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Attack.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Item/MDWeaponBase.h"
#include "../MakeDungeon.h"


UMDGA_Attack::UMDGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	
	//Refactoring
	/*UMDWeaponBase* Weapon = MDCharacter->GetWeapon();
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
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Attack::OnCompletedCallback);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Attack::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();

		StartComboTimer();
		
		MDCharacter->StopMovement();
	}*/

	MDCharacter->SetIsTrackingTarget(true);

	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
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

void UMDGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMDGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentWeaponAttackData = nullptr;
	CurrentCombo = 0;
	HasNextAttackInput = false;
}

void UMDGA_Attack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UMDGA_Attack::GetNextSection()
{
	FName NextSection;
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentWeaponAttackData->MaxComboCount);
	NextSection = *FString::Printf(TEXT("%s%d"), *CurrentWeaponAttackData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UMDGA_Attack::StartComboTimer()
{
	if (ComboTimerHandle.IsValid())
	{
		ComboTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMDGA_Attack::CheckComboInput, CurrentWeaponAttackData->InputThreshold, false);
}

void UMDGA_Attack::CheckComboInput()
{
	HasNextAttackInput = true;
}

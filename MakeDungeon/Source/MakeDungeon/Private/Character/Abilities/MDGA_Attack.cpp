// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Attack.h"
#include "Character/MDCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/MDAttackMontageData.h"
#include "../MakeDungeon.h"


UMDGA_Attack::UMDGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MD_LOG(LogMD, Log, TEXT("ActivateAbility"));

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	
	CurrentAttackMontageData = MDCharacter->GetAttackMontageData();

	UAnimMontage* AttackMontage = MDCharacter->GetAttackMontage();

	if (AttackMontage)
	{
		MD_LOG(LogMD, Log, TEXT("PlayMontage"));
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.f, GetNextSection());
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_Attack::OnCompletedCallback);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_Attack::OnInterruptedCallback);
		PlayAttackMontageTask->ReadyForActivation();

		StartComboTimer();
	}
}

void UMDGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//HasNextAttackInput = ComboTimerHandle.IsValid() ? true : false;
	if (!ComboTimerHandle.IsValid())
	{
		HasNextAttackInput = false;
		MD_LOG(LogMD, Log, TEXT("InputAbility_False"));
	}
	else
	{
		HasNextAttackInput = true;
		MD_LOG(LogMD, Log, TEXT("InputAbility_True"));
	}
	
}

void UMDGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMDGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentAttackMontageData = nullptr;
	CurrentCombo = 0;
	HasNextAttackInput = false;
	MD_LOG(LogMD, Log, TEXT("EndAbility"));
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

FName UMDGA_Attack::GetNextSection(bool bIsSecondary)
{
	FName NextSection;
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentAttackMontageData->MaxComboCount);
	NextSection = *FString::Printf(TEXT("%s%d"), *CurrentAttackMontageData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UMDGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentAttackMontageData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = CurrentAttackMontageData->EffectiveFrameCount[ComboIndex] / CurrentAttackMontageData->FrameRate;

	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMDGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UMDGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();

	if (HasNextAttackInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextAttackInput = false;
	}
}

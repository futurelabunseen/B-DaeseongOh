// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_PlayAnim_Combo.h"
#include "Character/MDCharacterBase.h"
#include "Item/MDWeaponBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_PlayAnim_Combo::UMDGA_PlayAnim_Combo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_PlayAnim_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();

	FGameplayTagContainer CurrentOwnedTags;
	ASC->GetOwnedGameplayTags(CurrentOwnedTags);

	FGameplayTag TargetTag = MDTAG_WEAPON_ATTACK;
	FGameplayTag ResultTag;
	for (FGameplayTag Tag : CurrentOwnedTags)
	{
		if (Tag.MatchesTag(TargetTag))
		{
			ResultTag = Tag;
		}
	}

	if (!ResultTag.IsValid() || ResultTag.MatchesTagExact(TargetTag))
	{
		OnInterrupted();
		MD_LOG(LogMD, Log, TEXT("No Anim"));
		return;
	}

	UMDWeaponBase* Weapon = MDCharacter->GetWeapon();

	CurrentComboAttackData = Weapon->GetComboAttackData(ResultTag);
	if (!CurrentComboAttackData)
	{
		OnInterrupted();
		MD_LOG(LogMD, Log, TEXT("No ComboAttackData"));
		return;
	}

	UAnimMontage* AttackMontage = Weapon->GetMontage(ResultTag);
	if (AttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.f, GetNextSection());
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_PlayAnim_Combo::OnCompleted);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_PlayAnim_Combo::OnInterrupted);
		PlayAttackMontageTask->ReadyForActivation();

		StartComboTimer();

		MDCharacter->StopMovement();
	}

	MDCharacter->SetIsTrackingTarget(true);

	MD_LOG(LogMD, Log, TEXT("Activate"));
}

void UMDGA_PlayAnim_Combo::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	MD_LOG(LogMD, Log, TEXT("InputPressed"));
	if (HasNextAttackInput && CurrentCombo < CurrentComboAttackData->MaxComboCount)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextAttackInput = false;
		MD_LOG(LogMD, Log, TEXT("NextAnim"));
	}
}

void UMDGA_PlayAnim_Combo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	FGameplayTagContainer CancelAbilityTags = MDTAG_WEAPON_ATTACK.GetSingleTagContainer();
	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&CancelAbilityTags);

	CurrentComboAttackData = nullptr;
	CurrentCombo = 0;
	HasNextAttackInput = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_PlayAnim_Combo::OnCompleted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_PlayAnim_Combo::OnInterrupted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UMDGA_PlayAnim_Combo::GetNextSection()
{
	FName NextSection;
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboAttackData->MaxComboCount);
	NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboAttackData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UMDGA_PlayAnim_Combo::StartComboTimer()
{
	if (ComboTimerHandle.IsValid())
	{
		ComboTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMDGA_PlayAnim_Combo::CheckComboInput, CurrentComboAttackData->InputThreshold, false);
}

void UMDGA_PlayAnim_Combo::CheckComboInput()
{
	HasNextAttackInput = true;
}

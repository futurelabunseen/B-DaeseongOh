// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Sword_ChargeSwing.h"
#include "Character/MDCharacterBase.h"
#include "Animation/MDAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_Sword_ChargeSwing::UMDGA_Sword_ChargeSwing()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UMDGA_Sword_ChargeSwing::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	// For Anim
	MDCharacter->SetIsCharged(true);
	UMDAnimInstance* AnimInst = Cast<UMDAnimInstance>(ActorInfo->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetAnimPlaySpeed(AnimPlaySpeed);
	}

	// For Attack
	Radius = 250.f;
	SpawnLocation = MDCharacter->GetActorLocation();

	// For Effect
	FGameplayEffectSpecHandle SkillInitEffectHandle = MakeOutgoingGameplayEffectSpec(SkillInitEffect);
	if (SkillInitEffectHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SkillInitEffectHandle);
	}

	MDCharacter->SetIsTrackingTarget(true);
}

void UMDGA_Sword_ChargeSwing::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	// For Attack
	Radius = FMath::Clamp(Radius + 1.f, Radius, 350.f);
	SpawnLocation = MDCharacter->GetActorLocation() + MDCharacter->GetActorForwardVector() * Radius * 0.8f;
	DrawDebugSphere(GetWorld(), SpawnLocation, Radius, 16, FColor::Cyan, false, 0.2f);
	MD_LOG(LogMD, Log, TEXT("CurrentAngle : %f"), Radius);
}

void UMDGA_Sword_ChargeSwing::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	// For Anim

	// For Attack
	MDCharacter->SetIsTrackingTarget(false);
	DrawDebugSphere(GetWorld(), SpawnLocation, Radius, 16, FColor::Green, false, 1.f);

	FGameplayEventData PayloadData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TriggerGameplayTag, PayloadData);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMDGA_Sword_ChargeSwing::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(CurrentActorInfo->AvatarActor.Get());
	MDCharacter->SetIsCharged(false);

	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(MDTAG_EVENT_CHARACTER_TRACESKILL.GetSingleTagContainer());

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
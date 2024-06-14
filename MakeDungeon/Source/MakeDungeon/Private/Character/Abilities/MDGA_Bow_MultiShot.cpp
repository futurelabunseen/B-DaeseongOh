// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_MultiShot.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Animation/MDAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Item/MDWeaponBow.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_Bow_MultiShot::UMDGA_Bow_MultiShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	Range = 1000.f;
	SocketName = FName("Weapon_R");
}

void UMDGA_Bow_MultiShot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	// For Anim
	MDCharacter->SetIsCharging(true);
	UMDAnimInstance* AnimInst = Cast<UMDAnimInstance>(ActorInfo->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetAnimPlaySpeed(AnimPlaySpeed);
	}

	UMDWeaponBow* MDBow = CastChecked<UMDWeaponBow>(MDCharacter->GetWeapon());

	// For Attack
	OuterAngle = MDBow->GetMultiShotMaxAngle();
	DecreaseAngle = OuterAngle * 0.02;

	MDCharacter->SetIsTrackingTarget(true);
}

void UMDGA_Bow_MultiShot::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// For Attack
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	MDCharacter->SetIsCharging(true);
	OuterAngle = FMath::Clamp(OuterAngle - DecreaseAngle, 1.0, OuterAngle);
	MD_LOG(LogMD, Log, TEXT("CurrentAngle : %lf"), OuterAngle);
}

void UMDGA_Bow_MultiShot::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	
	// For Anim

	// For Attack
	UMDWeaponBow* MDBow = CastChecked<UMDWeaponBow>(MDCharacter->GetWeapon());
	MDBow->SetMultiShotCurrentAngle(OuterAngle);

	MDCharacter->SetIsTrackingTarget(false);

	MDCharacter->SetIsCharging(false);
}

void UMDGA_Bow_MultiShot::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, false);
}

void UMDGA_Bow_MultiShot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

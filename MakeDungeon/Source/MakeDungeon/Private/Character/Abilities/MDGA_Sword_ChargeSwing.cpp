// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Sword_ChargeSwing.h"
#include "Character/MDCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "../MakeDungeon.h"

UMDGA_Sword_ChargeSwing::UMDGA_Sword_ChargeSwing()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Sword_ChargeSwing::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Radius = 250.f;
	AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	SpawnLocation = MDCharacter->GetActorLocation();
	MDCharacter->SetIsTrackingTarget(true);
}

void UMDGA_Sword_ChargeSwing::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	Radius = FMath::Clamp(Radius + 1.f, Radius, 350.f);
	SpawnLocation = MDCharacter->GetActorLocation() + MDCharacter->GetActorForwardVector() * Radius * 0.8f;
	DrawDebugSphere(GetWorld(), SpawnLocation, Radius, 16, FColor::Cyan, false, 0.2f);
	MD_LOG(LogMD, Log, TEXT("CurrentAngle : %f"), Radius);
}

void UMDGA_Sword_ChargeSwing::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	MDCharacter->SetIsTrackingTarget(false);
	DrawDebugSphere(GetWorld(), SpawnLocation, Radius, 16, FColor::Green, false, 1.f);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

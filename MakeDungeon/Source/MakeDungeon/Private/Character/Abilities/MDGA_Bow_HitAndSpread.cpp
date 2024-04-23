// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_HitAndSpread.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Kismet/KismetMathLibrary.h"


UMDGA_Bow_HitAndSpread::UMDGA_Bow_HitAndSpread()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Bow_HitAndSpread::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMDCharacterBase* SpawnInstigator = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	
	FRotator Direction = SpawnInstigator->GetAttackDirection();

	AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
		SpawnInstigator, SpawnInstigator->GetActorLocation(), Direction, 1000.f, EProjectileType::Spread);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

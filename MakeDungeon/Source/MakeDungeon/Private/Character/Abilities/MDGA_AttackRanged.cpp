// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AttackRanged.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
//#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Tags/MDGameplayTag.h"
#include "Game/ObjectPoolWorldSubsystem.h"

UMDGA_AttackRanged::UMDGA_AttackRanged()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	Range = 1000.f;
	SocketName = FName("Weapon_R");
}

void UMDGA_AttackRanged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TriggerEventData->EventTag);
	Task->EventReceived.AddDynamic(this, &UMDGA_AttackRanged::ShootBullet);
	Task->ReadyForActivation();*/

	//ShootBullet(CurrentEventData);
	AMDCharacterBase* SpawnInstigator = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());

	FRotator Direction = SpawnInstigator->GetAttackDirection();

	AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
		SpawnInstigator, SpawnInstigator->GetActorLocation(), Direction, 1000.f, EProjectileType::Normal);

	SpawnInstigator->SetIsTrackingTarget(false);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

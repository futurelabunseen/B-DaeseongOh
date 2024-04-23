// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_MultiShot.h"
#include "Character/MDCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
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
	OuterAngle = 60.0;
	DecreaseAngle = OuterAngle * 0.02;
}

void UMDGA_Bow_MultiShot::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	OuterAngle = FMath::Clamp(OuterAngle - DecreaseAngle, 1.0, OuterAngle);
	MD_LOG(LogMD, Log, TEXT("CurrentAngle : %lf"), OuterAngle);
}

void UMDGA_Bow_MultiShot::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UWorld* SpawnWorld = GetWorld();
	AActor* SpawnOwner = GetOwningActorFromActorInfo();
	AMDCharacterBase* SpawnInstigator = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	FVector SpawnLocation = SpawnInstigator->GetActorLocation();
	FRotator DirectionOrigin = SpawnInstigator->GetAttackDirection();

	FRotator DirectionSpawn[5] = { FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw, DirectionOrigin.Roll), 
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + OuterAngle * 0.5, DirectionOrigin.Roll),
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + OuterAngle, DirectionOrigin.Roll),
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + -(OuterAngle * 0.5), DirectionOrigin.Roll),
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + -OuterAngle, DirectionOrigin.Roll) };

	AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
		SpawnInstigator, SpawnLocation, DirectionSpawn[0], 1000.f, EProjectileType::Normal);
	AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
		SpawnInstigator, SpawnLocation, DirectionSpawn[1], 1000.f, EProjectileType::Normal);
	AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
		SpawnInstigator, SpawnLocation, DirectionSpawn[2], 1000.f, EProjectileType::Normal);
	AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
		SpawnInstigator, SpawnLocation, DirectionSpawn[3], 1000.f, EProjectileType::Normal);
	AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
		SpawnInstigator, SpawnLocation, DirectionSpawn[4], 1000.f, EProjectileType::Normal);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_MultiShot.h"
#include "Character/MDCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

UMDGA_Bow_MultiShot::UMDGA_Bow_MultiShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	Range = 1000.f;
	SocketName = FName("Weapon_R");
}

void UMDGA_Bow_MultiShot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ShootBullet(CurrentEventData, FVector(0.0, 0.0, 0.0));
	ShootBullet(CurrentEventData, FVector(0.0, 20.0, 0.0));
	ShootBullet(CurrentEventData, FVector(0.0, 40.0, 0.0));
	ShootBullet(CurrentEventData, FVector(0.0, -20.0, 0.0));
	ShootBullet(CurrentEventData, FVector(0.0, -40.0, 0.0));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMDGA_Bow_MultiShot::ShootBullet(FGameplayEventData EventData, const FVector& DeltaPitchYawRoll)
{
	AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	if (!MDCharacter)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	FVector Start = MDCharacter->GetMesh()->GetSocketLocation(SocketName);
	FVector End = MDCharacter->GetActorLocation() + MDCharacter->GetActorForwardVector() * Range;
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	Rotation.Add(DeltaPitchYawRoll.X, DeltaPitchYawRoll.Y, DeltaPitchYawRoll.Z);

	FTransform MuzzleTransform = MDCharacter->GetMesh()->GetSocketTransform(SocketName);
	MuzzleTransform.SetRotation(Rotation.Quaternion());
	MuzzleTransform.SetScale3D(FVector(1.f));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//SpawnParameters.Owner = GetOwningActorFromActorInfo();
	//SpawnParameters.Instigator = MDCharacter;

	
	//AMDProjectile* Projectile = GetWorld()->SpawnActor<AMDProjectile>(ProjectileClass, MuzzleTransform, SpawnParameters);
	//AMDProjectile* Projectile = GetWorld()->SpawnActorAbsolute<AMDProjectile>(ProjectileClass, MuzzleTransform, SpawnParameters);
	AMDProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMDProjectile>(ProjectileClass, MuzzleTransform, 
						GetOwningActorFromActorInfo(), MDCharacter, SpawnParameters.SpawnCollisionHandlingOverride);
	Projectile->Range = Range;
	Projectile->FinishSpawning(MuzzleTransform);
}

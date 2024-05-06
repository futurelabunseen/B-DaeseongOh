// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_MultiShot.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
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
	OuterAngle = 60.0;
	DecreaseAngle = OuterAngle * 0.02;

	AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
	MDCharacter->SetIsTrackingTarget(true);
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

	AMDProjectile* SpawnedProjectile = nullptr;
	for (uint8 SpawnLocationIndex = 0; SpawnLocationIndex < 5; SpawnLocationIndex++)
	{
		SpawnedProjectile = AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
			SpawnInstigator, SpawnLocation, DirectionSpawn[SpawnLocationIndex], 1000.f, EProjectileType::Normal);

		if(SpawnedProjectile)
		{
			SpawnedProjectile->GetCollisionComp()->OnComponentBeginOverlap.AddDynamic(this, &UMDGA_Bow_MultiShot::OnBeginOverlap);
		}
	}
	
	SpawnedProjectile = nullptr;

	SpawnInstigator->SetIsTrackingTarget(false);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMDGA_Bow_MultiShot::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != GetOwningActorFromActorInfo()))
	{
		AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(OtherActor);
		if (MDCharacter)
		{
			UAbilitySystemComponent* TargetASC = MDCharacter->GetAbilitySystemComponent();
			if (!TargetASC)
			{
				MD_LOG(LogMD, Error, TEXT("ASC not found!"));
				return;
			}

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
			if (EffectSpecHandle.IsValid())
			{
				FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(SweepResult);
				FGameplayAbilityTargetDataHandle TargetDataHandle;
				TargetDataHandle.Add(TargetData);
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(SweepResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;

				TargetASC->ExecuteGameplayCue(MDTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);

				MD_LOG(LogMD, Warning, TEXT("Hit!"));
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_MultiShot.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Animation/MDAnimInstance.h"
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

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	// For Anim
	MDCharacter->SetIsCharging(true);
	UMDAnimInstance* AnimInst = Cast<UMDAnimInstance>(ActorInfo->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->SetAnimPlaySpeed(AnimPlaySpeed);
	}


	// For Attack
	OuterAngle = 60.0;
	DecreaseAngle = OuterAngle * 0.02;

	MDCharacter->SetIsTrackingTarget(true);
}

void UMDGA_Bow_MultiShot::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// For Attack
	OuterAngle = FMath::Clamp(OuterAngle - DecreaseAngle, 1.0, OuterAngle);
	MD_LOG(LogMD, Log, TEXT("CurrentAngle : %lf"), OuterAngle);
}

void UMDGA_Bow_MultiShot::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());
	
	// For Anim

	// For Attack
	UWorld* SpawnWorld = GetWorld();
	AActor* SpawnOwner = ActorInfo->OwnerActor.Get();
	FVector SpawnLocation = MDCharacter->GetActorLocation();
	FRotator DirectionOrigin = MDCharacter->GetAttackDirection();

	FRotator DirectionSpawn[5] = { FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw, DirectionOrigin.Roll), 
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + OuterAngle * 0.5, DirectionOrigin.Roll),
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + OuterAngle, DirectionOrigin.Roll),
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + -(OuterAngle * 0.5), DirectionOrigin.Roll),
									FRotator(DirectionOrigin.Pitch, DirectionOrigin.Yaw + -OuterAngle, DirectionOrigin.Roll) };

	AMDProjectile* SpawnedProjectile = nullptr;
	for (uint8 SpawnLocationIndex = 0; SpawnLocationIndex < 5; SpawnLocationIndex++)
	{
		SpawnedProjectile = AMDProjectile::ShootProjectile(SpawnWorld, ProjectileClass, SpawnOwner,
			MDCharacter, SpawnLocation, DirectionSpawn[SpawnLocationIndex], 3000.f, EProjectileType::Normal);

		if(SpawnedProjectile)
		{
			SpawnedProjectile->GetCollisionComp()->OnComponentBeginOverlap.AddDynamic(this, &UMDGA_Bow_MultiShot::OnBeginOverlap);
		}
	}
	
	SpawnedProjectile = nullptr;

	MDCharacter->SetIsTrackingTarget(false);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMDGA_Bow_MultiShot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(CurrentActorInfo->AvatarActor.Get());
	MDCharacter->SetIsCharging(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Bow_MultiShot::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != GetOwningActorFromActorInfo()))
	{
		AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(CurrentActorInfo->AvatarActor.Get());
		if (MDCharacter)
		{
			UAbilitySystemComponent* TargetASC = MDCharacter->GetAbilitySystemComponent();
			if (!TargetASC)
			{
				MD_LOG(LogMD, Error, TEXT("ASC not found!"));
				return;
			}

			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(SweepResult);
			FGameplayAbilityTargetDataHandle TargetDataHandle;
			TargetDataHandle.Add(TargetData);

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
			if (EffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(SweepResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;

				TargetASC->ExecuteGameplayCue(MDTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);

				MD_LOG(LogMD, Warning, TEXT("Hit!"));
			}

			FGameplayEffectSpecHandle DebuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TargetDebuffEffect);
			if (DebuffEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DebuffEffectSpecHandle, TargetDataHandle);
			}
		}
	}
}

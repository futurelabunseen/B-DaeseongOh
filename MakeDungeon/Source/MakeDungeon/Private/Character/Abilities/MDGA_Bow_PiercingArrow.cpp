// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bow_PiercingArrow.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_Bow_PiercingArrow::UMDGA_Bow_PiercingArrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Bow_PiercingArrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	FRotator Direction = MDCharacter->GetAttackDirection();

	AMDProjectile* SpawnedProjectile = AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
		MDCharacter, MDCharacter->GetActorLocation(), Direction, 3000.f, EProjectileType::Pierce);

	if (SpawnedProjectile)
	{
		SpawnedProjectile->GetCollisionComp()->OnComponentBeginOverlap.AddDynamic(this, &UMDGA_Bow_PiercingArrow::OnBeginOverlap);
	}
	
	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UMDGA_Bow_PiercingArrow::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	MD_LOG(LogMD, Log, TEXT("Pressed %d"), AttackCount);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	FRotator Direction = MDCharacter->GetAttackDirection();

	Direction.Add(AttackCount * 30.0, 0.0, 0.0);

	++AttackCount;

	AMDProjectile* SpawnedProjectile = AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
		MDCharacter, MDCharacter->GetActorLocation(), Direction, 3000.f, EProjectileType::Pierce);

	if (3 <= AttackCount)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UMDGA_Bow_PiercingArrow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AttackCount = 1;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_Bow_PiercingArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

			for (auto TargetDebuffEffect : TargetDebuffEffects)
			{
				FGameplayEffectSpecHandle DebuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TargetDebuffEffect);
				if (DebuffEffectSpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DebuffEffectSpecHandle, TargetDataHandle);
				}
			}
		}
	}
}

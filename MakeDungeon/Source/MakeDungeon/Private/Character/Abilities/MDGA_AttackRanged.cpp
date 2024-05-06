// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AttackRanged.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/ObjectPoolWorldSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

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

	AMDProjectile* SpawnProjectile = nullptr;
	SpawnProjectile = AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
		SpawnInstigator, SpawnInstigator->GetActorLocation(), Direction, 1000.f, EProjectileType::Normal);

	if (SpawnProjectile)
	{
		SpawnProjectile->GetCollisionComp()->OnComponentBeginOverlap.AddDynamic(this, &UMDGA_AttackRanged::OnBeginOverlap);
	}

	SpawnInstigator->SetIsTrackingTarget(false);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
void UMDGA_AttackRanged::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

		//UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();
		//if (ASC)
		//{
		//	//ActorLineTraceSingle
		//	//GetWorld()->OverlapMultiByChannel()
		//	//UKismetSystemLibrary::SphereOverlapActors()
		//	//UKismetSystemLibrary::SphereTraceSingle()
		//}

		/*CollisionDisable();

		UObjectPoolWorldSubsystem* ObjectPool = UWorld::GetSubsystem<UObjectPoolWorldSubsystem>(GetWorld());
		ObjectPool->CollectObject(this);*/

		MD_LOG(LogMD, Log, TEXT("Collect_Overlap"));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_Bullet_HitAndSpread.h"
#include "Character/MDCharacterBase.h"
#include "Character/MDProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tags/MDGameplayTag.h"
#include "../MakeDungeon.h"

UMDGA_Bullet_HitAndSpread::UMDGA_Bullet_HitAndSpread()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_Bullet_HitAndSpread::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	bIsFirst = true;

	AMDCharacterBase* SpawnInstigator = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());

	FRotator Direction = SpawnInstigator->GetAttackDirection();

	AMDProjectile* SpawnedProjectile = AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
		SpawnInstigator, SpawnInstigator->GetActorLocation(), Direction, 3000.f, EProjectileType::Spread);

	if (SpawnedProjectile)
	{
		SpawnedProjectile->GetCollisionComp()->OnComponentBeginOverlap.AddDynamic(this, &UMDGA_Bullet_HitAndSpread::OnBeginOverlap);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMDGA_Bullet_HitAndSpread::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != GetAvatarActorFromActorInfo()))
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

			FGameplayEffectSpecHandle DebuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TargetDebuffEffect);
			if (DebuffEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DebuffEffectSpecHandle, TargetDataHandle);
				
				/*FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(SweepResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;
				
				TargetASC->ExecuteGameplayCue(WeaponDebuff, CueParam);*/
			}

			AMDProjectile* Projectile = Cast<AMDProjectile>(OverlappedComponent->GetOwner());
			if (Projectile && EProjectileType::Spread == Projectile->GetProjectileType())
			{
				FVector StartLocation = MDCharacter->GetActorLocation();

				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				TEnumAsByte PhysicsBody = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
				TEnumAsByte Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
				ObjectTypes.Add(PhysicsBody);
				ObjectTypes.Add(Pawn);

				UClass* ClassFilter = AMDCharacterBase::StaticClass();

				TArray<AActor*> ActorToIgnore;
				ActorToIgnore.Add(OtherActor);
				ActorToIgnore.Add(GetAvatarActorFromActorInfo());

				TArray<AActor*> ResultActors;

				UKismetSystemLibrary::SphereOverlapActors(GetWorld(), StartLocation, 700.f,
					ObjectTypes, ClassFilter, ActorToIgnore, ResultActors);
				DrawDebugSphere(GetWorld(), StartLocation, 700.f, 16, FColor::Blue, false, 2.f);

				AMDCharacterBase* SpawnInstigator = Cast<AMDCharacterBase>(GetAvatarActorFromActorInfo());
				AMDProjectile* SpawnedProjectile = nullptr;
				for (auto& TargetActor : ResultActors)
				{
					FRotator Direction = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetActor->GetActorLocation());

					SpawnedProjectile = AMDProjectile::ShootProjectile(GetWorld(), ProjectileClass, GetOwningActorFromActorInfo(),
						SpawnInstigator, StartLocation, Direction, 3000.f, EProjectileType::Normal, OtherActor);

					if (SpawnedProjectile)
					{
						SpawnedProjectile->GetCollisionComp()->OnComponentBeginOverlap.AddDynamic(this, &UMDGA_Bullet_HitAndSpread::OnBeginOverlap);
					}
				}
			}
		}
	}
}

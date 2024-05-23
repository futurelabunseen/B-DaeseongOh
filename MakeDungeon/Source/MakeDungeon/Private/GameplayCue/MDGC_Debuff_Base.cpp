// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/MDGC_Debuff_Base.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

AMDGC_Debuff_Base::AMDGC_Debuff_Base()
{
    ParticleSystem = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleSystem"));
}

bool AMDGC_Debuff_Base::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else
	{
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
			{
				//Spawn || Activate
				UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}

	return false;
}

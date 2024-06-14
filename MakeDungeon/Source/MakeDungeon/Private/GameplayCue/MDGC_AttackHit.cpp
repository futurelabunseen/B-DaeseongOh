// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/MDGC_AttackHit.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UMDGC_AttackHit::UMDGC_AttackHit()
{
}

bool UMDGC_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraFX, HitResult->ImpactPoint, FRotator::ZeroRotator);
		//UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraFX, HitResult->ImpactPoint, FRotator::ZeroRotator);
		//UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else
	{
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
			{
				//Spawn || Activate
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraFX, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator);
				//UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraFX, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator);
				//UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}

	return false;
}

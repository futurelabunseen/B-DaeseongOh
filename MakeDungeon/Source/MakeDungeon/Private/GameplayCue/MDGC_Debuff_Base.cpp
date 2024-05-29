// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/MDGC_Debuff_Base.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AMDGC_Debuff_Base::AMDGC_Debuff_Base()
{
}

//bool AMDGC_Debuff_Base::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
//{
//	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
//	if (HitResult)
//	{
//		NiagaraFX->Activate();
//		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraFX->GetAsset(), HitResult->ImpactPoint, FRotator::ZeroRotator);
//	}
//	else
//	{
//		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
//		{
//			if (TargetActor.Get())
//			{
//				//Spawn || Activate
//				//UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraFX, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator);
//				NiagaraFX->Activate();
//			}
//		}
//	}
//
//	return false;
//}
//
//bool AMDGC_Debuff_Base::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
//{
//	NiagaraFX->Deactivate();
//
//	return false;
//}

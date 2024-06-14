// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Tasks/MDAT_Trace.h"
#include "Character/Abilities/TargetActors/MDTA_Trace.h"
#include "AbilitySystemComponent.h"

UMDAT_Trace::UMDAT_Trace()
{
}

UMDAT_Trace* UMDAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AMDTA_Trace> TargetActorClass, const FVector& SpawnLocation)
{
	UMDAT_Trace* NewTask = NewAbilityTask<UMDAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	NewTask->SpawnLocation = SpawnLocation;
	return NewTask;
}

void UMDAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UMDAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UMDAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AMDTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UMDAT_Trace::OnTargetDataReadyCallback);
	}
}

void UMDAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability, SpawnLocation);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UMDAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}

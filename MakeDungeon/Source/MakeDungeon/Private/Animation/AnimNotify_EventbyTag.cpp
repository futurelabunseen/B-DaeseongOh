// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EventbyTag.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_EventbyTag::UAnimNotify_EventbyTag()
{
}

FString UAnimNotify_EventbyTag::GetNotifyName_Implementation() const
{
	return TEXT("EventbyTag");
}

void UAnimNotify_EventbyTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData Payload;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, Payload);
		}
	}
}

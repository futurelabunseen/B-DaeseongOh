// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EventByTag.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_EventByTag::UAnimNotify_EventByTag()
{
}

FString UAnimNotify_EventByTag::GetNotifyName_Implementation() const
{
	return TEXT("EventByTag");
}

void UAnimNotify_EventByTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

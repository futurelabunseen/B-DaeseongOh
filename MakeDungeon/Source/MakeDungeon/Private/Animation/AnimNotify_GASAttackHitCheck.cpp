// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/MDCharacterBase.h"

UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
{
	ComboAttackLevel = 1.f;
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(MeshComp->GetOwner());
		if (MDCharacter)
		{
			UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();

			FGameplayEventData PayloadData;
			PayloadData.ContextHandle = ASC->MakeEffectContext();
			PayloadData.ContextHandle.AddOrigin(SpawnLocation);
			PayloadData.EventMagnitude = ComboAttackLevel;
			PayloadData.OptionalObject = Cast<UObject>(VFX);
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MDCharacter, TriggerGameplayTag, PayloadData);

			
		}
	}
}

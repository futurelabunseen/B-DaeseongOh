// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_CancelAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/MDCharacterBase.h"
#include "../MakeDungeon.h"

UAnimNotify_CancelAbility::UAnimNotify_CancelAbility()
{
}

FString UAnimNotify_CancelAbility::GetNotifyName_Implementation() const
{
	return TEXT("CancelAbility");
}

void UAnimNotify_CancelAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(MeshComp->GetOwner());
		if (MDCharacter)
		{
			UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();

			TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

			for (auto& Spec : ActivatableAbilities)
			{
				if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(CancelGameplayTag))
				{
					if (Spec.IsActive())
					{
						ASC->CancelAbilityHandle(Spec.Handle);
						MD_LOG(LogMD, Log, TEXT("Canceled"));
					}
				}
			}
		}
	}
}

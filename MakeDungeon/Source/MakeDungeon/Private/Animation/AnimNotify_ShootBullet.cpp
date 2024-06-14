// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShootBullet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/MDCharacterBase.h"
#include "Item/MDWeaponBow.h"

UAnimNotify_ShootBullet::UAnimNotify_ShootBullet()
{
	DireactionYaw = 0.f;
}

FString UAnimNotify_ShootBullet::GetNotifyName_Implementation() const
{
	return TEXT("ShootBullet");
}

void UAnimNotify_ShootBullet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(MeshComp->GetOwner());
		if (MDCharacter)
		{
			UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();
			
			UMDWeaponBow* MDBow = CastChecked<UMDWeaponBow>(MDCharacter->GetWeapon());

			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = MDBow->GetMultiShotCurrentAngle() * DireactionYaw;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MDCharacter, TriggerGameplayTag, PayloadData);
		}
	}
}

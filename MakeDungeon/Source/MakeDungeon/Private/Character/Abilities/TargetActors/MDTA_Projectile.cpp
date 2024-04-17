// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/TargetActors/MDTA_Projectile.h"
#include "AbilitySystemComponent.h"
#include "../MakeDungeon.h"

AMDTA_Projectile::AMDTA_Projectile()
{
}

void AMDTA_Projectile::StartTargeting(UGameplayAbility* Ability)
{
	/*Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();*/
}

void AMDTA_Projectile::ConfirmTargetingAndContinue()
{
	/*if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}*/
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/TargetActors/MDTA_SphereMultiTrace.h"
#include "Character/Abilities/AttributeSets/MDCharacterSkillAttributeSet.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/MDCollision.h"
#include "DrawDebugHelpers.h"
#include "../MakeDungeon.h"

FGameplayAbilityTargetDataHandle AMDTA_SphereMultiTrace::MakeTargetData() const
{
	MD_LOG(LogMD, Log, TEXT("Begin"));

	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);

	if (!ASC)
	{
		MD_LOG(LogMD, Error, TEXT("ASC not found"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UMDCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UMDCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		MD_LOG(LogMD, Error, TEXT("SkillAttribute not found"));
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = SkillAttribute->GetSkillRange();

	FVector Origin = Character->GetMesh()->GetSocketLocation(FName("Weapon_R_Muzzle"));
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AMDTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_MDACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}

	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16,DrawColor, false, 5.f);
	}
#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "AI/MDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AMDCharacterNonPlayer::AMDCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UMDCharacterAttributeSet>(TEXT("AttributeSet"));

	AIControllerClass = AMDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Level = 1;
}

void AMDCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	for (const auto& StartAbility : CharacterAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AMDCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	//GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//GetMesh()->AddImpulse(RagdollImpulse, NAME_None, true); // Ragdoll 충격 적용
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 5.f, false);
}

void AMDCharacterNonPlayer::OnOutOfHealth()
{
	Super::OnOutOfHealth();
}

float AMDCharacterNonPlayer::GetAIPatrolRadius()
{
	return 500.0f;
}

float AMDCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AMDCharacterNonPlayer::GetAIAttackRange()
{
	return AttributeSet->GetAttackRange() + AttributeSet->GetAttackRadius() * 2;
}

float AMDCharacterNonPlayer::GetAITurnSpeed()
{
	return 0.0f;
}

void AMDCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AMDCharacterNonPlayer::AttackByAI()
{
	//공격 로직 구현
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FGameplayEventData PayloadData;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AttackTriggerGameplayTag, PayloadData);

	//공격이 끝나면 실행
	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			OnAttackFinished.ExecuteIfBound();
		}
	), 2.f, false);
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Abilities/MDGA_AI_Attack.h"
#include "Interface/MDGameInterface.h"
#include "GameFramework/GameModeBase.h"

AMDCharacterNonPlayer::AMDCharacterNonPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UMDCharacterAttributeSet>(TEXT("AttributeSet"));

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
}

void AMDCharacterNonPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsDead())
	{
		if (2.f < DeadTime)
		{
			Destroy();
		}
		DeadTime += DeltaSeconds;
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
	
	IMDGameInterface* MDGameMode = Cast<IMDGameInterface>(GetWorld()->GetAuthGameMode());
	if (MDGameMode)
	{
		MDGameMode->OnPlayerScoreChanged(-1);
		if (MDGameMode->IsGameCleared())
		{
			return;
		}
	}

	/*FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			SetActorHiddenInGame(true);
		}
	), 0.5f, false);*/

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
	return 5000.0f;
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
	FGameplayEventData PayloadData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AttackTriggerGameplayTag, PayloadData);
}

void AMDCharacterNonPlayer::AttackAIFinish()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

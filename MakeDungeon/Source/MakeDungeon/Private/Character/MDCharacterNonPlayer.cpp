// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "AI/MDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Abilities/MDGA_AI_Attack.h"

AMDCharacterNonPlayer::AMDCharacterNonPlayer()
{
	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Private/ParagonKhaimera/Characters/Heroes/Khaimera/Skins/Tier2/GruxPelt/Meshes/Khaimera_GruxPelt.Khaimera_GruxPelt'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MakeDungeon/Animation/Monster/ABP_MDMonster.ABP_MDMonster_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

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
		/*if (StartSpec.Ability->AbilityTags.HasAny(FGameplayTag::RequestGameplayTag(FName("NonPlayer.Attack")).GetSingleTagContainer()))
		{
			UMDGA_AI_Attack* AIAttackAbility = Cast<UMDGA_AI_Attack>(StartSpec.Ability);
			AIAttackAbility->OnAbilityExecuted.AddDynamic(this, &AMDCharacterNonPlayer::AttackAIFinish);
		}*/
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
	
	/*TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(AttackTriggerGameplayTag))
		{
			if (Spec.IsActive())
			{
				UMDGA_AI_Attack* AIAttackAbility = Cast<UMDGA_AI_Attack>(Spec.Ability);
				AIAttackAbility->OnAbilityExecuted.AddDynamic(this, &AMDCharacterNonPlayer::AttackAIFinish);
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}*/

	//공격이 끝나면 실행
	/*FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			OnAttackFinished.ExecuteIfBound();
		}
	), 2.f, false);*/
	
}

void AMDCharacterNonPlayer::AttackAIFinish()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

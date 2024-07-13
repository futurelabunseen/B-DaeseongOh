// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_AI_Attack.h"
#include "Character/MDCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/MDAI.h"

UMDGA_AI_Attack::UMDGA_AI_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_AI_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotator = FRotator::ZeroRotator;

	AAIController* AIController = Cast<AAIController>(MDCharacter->GetController());
	if (AIController)
	{
		UBehaviorTreeComponent* AIBT = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
		if (AIBT)
		{
			APawn* TargetPawn = Cast<APawn>(AIBT->GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
			TargetLocation = TargetPawn->GetActorLocation();

			TargetRotator = FRotationMatrix::MakeFromX(TargetLocation - MDCharacter->GetActorLocation()).Rotator();
		}
	}

	MDCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if (Montage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Montage);
		PlayAttackMontageTask->OnCompleted.AddDynamic(this, &UMDGA_AI_Attack::OnCompleted);
		PlayAttackMontageTask->OnInterrupted.AddDynamic(this, &UMDGA_AI_Attack::OnInterrupted);
		PlayAttackMontageTask->ReadyForActivation();

		MDCharacter->StopMovement();
		MDCharacter->SetActorRotation(TargetRotator);
	}
}

void UMDGA_AI_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AMDCharacterBase* MDCharacter = CastChecked<AMDCharacterBase>(ActorInfo->AvatarActor.Get());

	AAIController* AIController = Cast<AAIController>(MDCharacter->GetController());
	if (AIController)
	{
		UBehaviorTreeComponent* AIBT = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
		if (AIBT)
		{
			const UBTNode* BTNode = AIBT->GetActiveNode();

			UBTTaskNode* TemplateNode = (UBTTaskNode*)AIBT->FindTemplateNode(BTNode);
			AIBT->OnTaskFinished(TemplateNode, EBTNodeResult::Succeeded);

		}
	}
	MDCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMDGA_AI_Attack::OnCompleted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_AI_Attack::OnInterrupted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

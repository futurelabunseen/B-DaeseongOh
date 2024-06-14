// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_CancelAbility.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UAnimNotify_CancelAbility : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_CancelAbility();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag CancelGameplayTag;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_EventbyTag.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UAnimNotify_EventbyTag : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_EventbyTag();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag; 

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MDItemBox.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class MAKEDUNGEON_API AMDItemBox : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AMDItemBox();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(AActor* Other) override;

protected:
	virtual void PostInitializeComponents() override;

	void ApplyEffectToTarget(AActor* Target);
	void InvokeGameplayCue(AActor* Target);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "GAS", meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;
};

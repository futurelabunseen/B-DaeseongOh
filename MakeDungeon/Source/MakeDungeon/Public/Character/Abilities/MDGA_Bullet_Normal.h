// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Character/MDProjectile.h"
#include "MDGA_Bullet_Normal.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_Bullet_Normal : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMDGA_Bullet_Normal();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMDProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EProjectileType ProjectileType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> TargetDebuffEffects;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag HitEffectTag;

private:
	double DirectionYaw;

	float Range;

	FName SocketName;
};

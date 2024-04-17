// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MAKEDUNGEON_API AMDProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMDProjectile();

	//FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComponent; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float Range;

	/*UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;*/

protected:
	virtual void BeginPlay() override;

	/*UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USphereComponent* CollisionComponent;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
private:
};

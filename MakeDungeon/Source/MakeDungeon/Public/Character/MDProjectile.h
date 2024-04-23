// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UENUM()
enum class EProjectileType : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Spread	UMETA(DisplayName = "Spread"),
	Mortar	UMETA(DisplayName = "Mortar")
};

UCLASS()
class MAKEDUNGEON_API AMDProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMDProjectile();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UStaticMesh> BulletMesh;

	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComponent; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	FORCEINLINE EProjectileType GetProjectileType() const { return ProjectileType; }
	FORCEINLINE void SetProjectileType(EProjectileType Type) { ProjectileType = Type; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float Range;

	static void ShootProjectile(UWorld* WorldContextObject, UClass* Class, AActor* ProjectileOwner, APawn* ProjectileInstigator, const FVector& StartPoint, const FRotator& Direction, float ProjectileRange, EProjectileType Type = EProjectileType::Normal);

	/*UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;*/

protected:
	virtual void BeginPlay() override;
	bool FindAroundTarget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	EProjectileType ProjectileType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* BulletMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBeginOverlapAndSpread(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDestroyedCallBack(AActor* DestroyedActor);

private:
	uint8 bIsOverlapped : 1;
};

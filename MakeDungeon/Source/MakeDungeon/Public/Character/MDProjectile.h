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
	Pierce	UMETA(DisplayName = "Pierce"),
	Mortar	UMETA(DisplayName = "Mortar")
};

UCLASS()
class MAKEDUNGEON_API AMDProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMDProjectile();

	static AMDProjectile* ShootProjectile(UWorld* WorldContextObject, UClass* Class, AActor* ProjectileOwner, APawn* ProjectileInstigator, const FVector& StartPoint, const FRotator& Direction, float InitSpeed, EProjectileType Type = EProjectileType::Normal, AActor* IgnoreActor = nullptr);

	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComponent; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	FORCEINLINE EProjectileType GetProjectileType() const { return ProjectileType; }
	FORCEINLINE void SetProjectileType(EProjectileType Type) { ProjectileType = Type; }

	FORCEINLINE void SetIgnoreTarget(AActor* IgnoreActor) { IgnoreTarget = IgnoreActor; }

protected:
	virtual void BeginPlay() override;
	void SetDead();

	bool FindAroundTarget();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBeginOverlap_Pierce(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDestroyedCallBack(AActor* DestroyedActor);

public:

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UStaticMesh> BulletMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	EProjectileType ProjectileType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* BulletMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	TObjectPtr<AActor> IgnoreTarget = nullptr;
private:

	uint8 bIsOverlapped : 1;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolWorldSubsystem.generated.h"

class AMDProjectile;
/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UObjectPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UObjectPoolWorldSubsystem();
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	AMDProjectile* ReuseObject(UClass* Class, const FVector& Location, const FRotator& Rotation);
	UFUNCTION(BlueprintCallable)
	void CollectObject(AMDProjectile* CollectObject);

protected:
	UPROPERTY()
	TArray<TObjectPtr<AMDProjectile>> ObjectPool;
};

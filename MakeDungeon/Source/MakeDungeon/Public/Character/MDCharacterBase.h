// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MDCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UMDCharacterAttributeSet;
class UMDWeaponBase;
class UMotionWarpingComponent;
class UMDWidgetComponent;

UCLASS()
class MAKEDUNGEON_API AMDCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMDCharacterBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TMap<FGameplayTag, TSubclassOf<UMDWeaponBase>> WeaponsInfo;

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE UMDWeaponBase* GetWeapon() const { return Weapons[CurrentWeapon]; }
	FORCEINLINE FGameplayTag GetWeaponType() const { return CurrentWeapon; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable)
	virtual FVector GetAttackLocation() const { return FVector(); }

	UFUNCTION(BlueprintCallable)
	virtual FRotator GetAttackDirection(bool GetCursorDirection = false) const;
	
	FORCEINLINE bool IsTrackingTarget() const { return bIsTrackingTarget; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsTrackingTarget(bool IsTrackingTarget) { bIsTrackingTarget = IsTrackingTarget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsCharging() { return bIsCharging; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsCharging(bool IsCharging) { bIsCharging = IsCharging; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLooping() { return bIsLooping; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsLooping(bool IsLooping) { bIsLooping = IsLooping; }

	FORCEINLINE bool IsDead() { return bIsDead; }

	virtual void StopMovement() {}

protected:
	virtual void SetDead();

	void InitWeapons();

	UFUNCTION()
	virtual void OnOutOfHealth();

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

	UPROPERTY()
	TObjectPtr<UMDCharacterAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UMDWeaponBase>> Weapons;

	FGameplayTag CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MWC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMDWidgetComponent> StatusBar;

private:
	float TrackingSpeed;
	uint8 bIsTrackingTarget : 1;
	uint8 bIsCharging : 1;
	uint8 bIsLooping : 1;
	uint8 bIsDead : 1;
};

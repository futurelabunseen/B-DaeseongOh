// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MDCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UMDCharacterAttributeSet;
class UMDWeaponBase;
class UMotionWarpingComponent;

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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//FORCEINLINE virtual UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	//FORCEINLINE class UMDAttackMontageData* GetAttackMontageData() const { return AttackMontageData; }
	FORCEINLINE UMDWeaponBase* GetWeapon() const { return Weapons[CurrentWeapon]; }
	FORCEINLINE FGameplayTag GetWeaponType() const { return CurrentWeapon; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable)
	virtual FVector GetAttackLocation() const { return FVector(); }
	UFUNCTION(BlueprintCallable)
	virtual FRotator GetAttackDirection() const;
	
	FORCEINLINE bool IsTrackingTarget() const { return bIsTrackingTarget; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsTrackingTarget(bool IsTrackingTarget) { bIsTrackingTarget = IsTrackingTarget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsCharged() { return bIsCharged; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsCharged(bool IsCharged) { bIsCharged = IsCharged; }

	virtual void StopMovement() {}

protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UMDWeaponAttackData> AttackMontageData;*/
	void InitWeapons();

	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	UPROPERTY()
	TObjectPtr<UMDCharacterAttributeSet> AttributeSet;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UMDWeaponBase>> Weapons;

	FGameplayTag CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MWC;

private:
	float TrackingSpeed;
	uint8 bIsTrackingTarget : 1;
	uint8 bIsCharged : 1;
};

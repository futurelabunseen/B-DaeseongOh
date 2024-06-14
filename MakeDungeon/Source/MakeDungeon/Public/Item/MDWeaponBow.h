// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MDWeaponBase.h"
#include "MDWeaponBow.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDWeaponBow : public UMDWeaponBase
{
	GENERATED_BODY()
	
public:
	UMDWeaponBow();

	float	GetMultiShotMaxAngle() const { return MultiShotMaxAngle; }
	float	GetMultiShotCurrentAngle() const { return MultiShotCurrentAngle; }
	void	SetMultiShotCurrentAngle(float Angle) { MultiShotCurrentAngle = Angle; }

	virtual void InitWeapon(AMDCharacterBase* InCharacter) override;

protected:
	UPROPERTY(EditAnywhere)
	float MultiShotMaxAngle;

	UPROPERTY()
	float MultiShotCurrentAngle;
};

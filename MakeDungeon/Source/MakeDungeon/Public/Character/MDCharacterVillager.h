// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MDCharacterNonPlayer.h"
#include "MDCharacterVillager.generated.h"

class UBoxComponent;
class UMDItemBaseData;
class UMDInventory;
/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API AMDCharacterVillager : public AMDCharacterNonPlayer
{
	GENERATED_BODY()
public:
	AMDCharacterVillager();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void VisibleShop();

	UFUNCTION()
	void OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> InteractionTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UMDInventory> MDShopWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
	TObjectPtr<UMDInventory> MDShopWidget;

	UPROPERTY(EditAnywhere, Category = "Shop")
	TArray<TObjectPtr<UMDItemBaseData>> ShopItems;
};

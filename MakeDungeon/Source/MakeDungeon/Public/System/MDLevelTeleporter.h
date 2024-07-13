// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MDLevelTeleporter.generated.h"

class UBoxComponent;

UCLASS()
class MAKEDUNGEON_API AMDLevelTeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	AMDLevelTeleporter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTeleportTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Teleporter;

	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> TeleportTrigger;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MDStageGimmick.generated.h"

class AMDCharacterNonPlayer;

UCLASS()
class MAKEDUNGEON_API AMDStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	AMDStageGimmick();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMonsterDestroyed(AActor* DestroyedActor);

	void OnMonsterSpawn();

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMDCharacterNonPlayer> MonsterClass;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = "true"))
	float BeginSpawnTime;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = "true"))
	float MonsterSpawnTime;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = "true"))
	double SpawnRange;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = "true"))
	int32 SpawnCount = 0;

	FTimerHandle MonsterSpawnTimerHandle;

private:
	int32 RemainingTime = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/MDStageGimmick.h"
#include "GameFramework/GameModeBase.h"
#include "Character/MDCharacterNonPlayer.h"
#include "Interface/MDGameInterface.h"
#include "Player/MDPlayerController.h"
#include "../MakeDungeon.h"

AMDStageGimmick::AMDStageGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	BeginSpawnTime = 3.f;
	MonsterSpawnTime = 30.f;
	SpawnRange = 10.0;

}

void AMDStageGimmick::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(MonsterSpawnTimerHandle, this,
					&AMDStageGimmick::OnMonsterSpawn, BeginSpawnTime, false);
}

void AMDStageGimmick::OnMonsterDestroyed(AActor* DestroyedActor)
{
	
}

void AMDStageGimmick::OnMonsterSpawn()
{
	double MinAngle, MaxAngle, RandAngle, LocationX, LocationY;
	double AngleRange = 2.0 / SpawnCount;

	int32 CurrentSpawnCount = SpawnCount;
	TArray<FVector> Locations;
	while(0 < CurrentSpawnCount)
	{
		MinAngle = (CurrentSpawnCount - 1) * AngleRange;
		MaxAngle = CurrentSpawnCount * AngleRange;

		RandAngle = FMath::RandRange(MinAngle, MaxAngle);
		LocationX = cos(RandAngle * PI) * SpawnRange;
		LocationY = sin(RandAngle * PI) * SpawnRange;

		MD_LOG(LogMD, Log, TEXT("Angle : %.2f, Min : %.2f Max : %.2f "), RandAngle, MinAngle, MaxAngle);

		FVector SpawnLocation = GetActorLocation();
		SpawnLocation += FVector(LocationX, LocationY, 10.0);
		Locations.Push(SpawnLocation);
		AActor* MonsterActor = GetWorld()->SpawnActor(MonsterClass, &SpawnLocation, &FRotator::ZeroRotator);
		AMDCharacterNonPlayer* MDMonster = Cast<AMDCharacterNonPlayer>(MonsterActor);
		if (MDMonster)
		{
			MDMonster->OnDestroyed.AddDynamic(this, &AMDStageGimmick::OnMonsterDestroyed);

			IMDGameInterface* MDGameMode = Cast<IMDGameInterface>(GetWorld()->GetAuthGameMode());
			if (MDGameMode)
			{
				MDGameMode->OnPlayerScoreChanged(1);
			}
		}
		else
		{
			MD_LOG(LogMD, Error, TEXT("Failed Spawn"));
			
			for (auto& Location : Locations)
			{
				MD_LOG(LogMD, Error, TEXT("Location : %.2f, %.2f, %.2f"), Location.X, Location.Y, Location.Z);
			}
		}
		--CurrentSpawnCount;
	}

	SpawnCount = FMath::RandRange(5, 10);
	GetWorld()->GetTimerManager().SetTimer(MonsterSpawnTimerHandle, this, &AMDStageGimmick::OnMonsterSpawn, MonsterSpawnTime, false);
	RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(MonsterSpawnTimerHandle);
}

void AMDStageGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterSpawnTimerHandle.IsValid())
	{
		int32 CurrentRemainigTime = GetWorld()->GetTimerManager().GetTimerRemaining(MonsterSpawnTimerHandle);

		if(RemainingTime != CurrentRemainigTime)
		{
			AMDPlayerController* MDPlayerController = Cast<AMDPlayerController>(GetWorld()->GetFirstPlayerController());

			if (MDPlayerController)
			{
				MDPlayerController->SpawnTimeChanged(CurrentRemainigTime);
				RemainingTime = CurrentRemainigTime;
			}
		}
	}

	
}


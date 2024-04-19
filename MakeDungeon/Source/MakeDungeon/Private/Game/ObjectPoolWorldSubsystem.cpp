// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ObjectPoolWorldSubsystem.h"
#include "Character/MDProjectile.h"
#include "../MakeDungeon.h"

UObjectPoolWorldSubsystem::UObjectPoolWorldSubsystem()
{

}

void UObjectPoolWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	/*FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform Transform(FTransform::Identity);
	UClass* ProjectileDefaultClass = AMDProjectile::StaticClass();
	
	ObjectPool.Reserve(200);

	int32 PoolNum = ObjectPool.Num();

	for (PoolNum; PoolNum < 200; ++PoolNum)
	{
		AMDProjectile* MDProjectile = GetWorld()->SpawnActor<AMDProjectile>(ProjectileDefaultClass, Transform, SpawnParam);
		ObjectPool.Add(MDProjectile);
	}*/
}

void UObjectPoolWorldSubsystem::Deinitialize()
{
	if(!ObjectPool.IsEmpty())
	{
		ObjectPool.Empty();
	}
}

AMDProjectile* UObjectPoolWorldSubsystem::ReuseObject(UClass* Class, const FVector& Location, const FRotator& Rotation)
{
	AMDProjectile* ReusedObject = nullptr;

	if (ObjectPool.IsEmpty())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ReusedObject = GetWorld()->SpawnActor<AMDProjectile>(Class, Location, Rotation, SpawnParam);

		MD_LOG(LogMD, Warning, TEXT("Spawn, Current Count : %d"), ObjectPool.Num());
	}
	else
	{
		ReusedObject = ObjectPool.Pop(false);
		if (!ReusedObject)
		{
			MD_LOG(LogMD, Warning, TEXT("Null, Current Count : %d"), ObjectPool.Num());
			return ReusedObject;
		}
		ReusedObject->SetActorLocationAndRotation(Location, Rotation);
		MD_LOG(LogMD, Warning, TEXT("Pool, Current Count : %d"), ObjectPool.Num());
	}

	return ReusedObject;
}

void UObjectPoolWorldSubsystem::CollectObject(AMDProjectile* CollectObject)
{
	check(CollectObject);
	CollectObject->SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	ObjectPool.Add(CollectObject);
}

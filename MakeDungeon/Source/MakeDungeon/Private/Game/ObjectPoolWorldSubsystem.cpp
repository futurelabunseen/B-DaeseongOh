// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ObjectPoolWorldSubsystem.h"
#include "Character/MDProjectile.h"

UObjectPoolWorldSubsystem::UObjectPoolWorldSubsystem()
	: ObjectCount(0)
{

}

void UObjectPoolWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	/*if (ObjectPool.IsEmpty())
	{
		ObjectCount = 0;
	}

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform Transform(FTransform::Identity);

	for (ObjectCount; ObjectCount < 500; ++ObjectCount)
	{
		ObjectPool.Enqueue(GetWorld()->SpawnActor<AMDProjectile>(AMDProjectile::StaticClass(), Transform, SpawnParam));
	}*/
}

void UObjectPoolWorldSubsystem::Deinitialize()
{
	if(!ObjectPool.IsEmpty())
	{
		ObjectPool.Empty();
		ObjectCount = 0;
	}
}

AMDProjectile* UObjectPoolWorldSubsystem::ReuseObject(const FTransform& Transform)
{
	TObjectPtr<AMDProjectile> ReusedObject = nullptr;

	if (ObjectPool.IsEmpty())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ReusedObject = GetWorld()->SpawnActor<AMDProjectile>(AMDProjectile::StaticClass(), Transform, SpawnParam);
	}
	else
	{
		if(ObjectPool.Dequeue(ReusedObject))
		{
			ReusedObject->SetActorTransform(Transform);
		}
	}

	return ReusedObject;
}

void UObjectPoolWorldSubsystem::CollectObject(AMDProjectile* CollectObject)
{
	check(CollectObject);

	ObjectPool.Enqueue(CollectObject);
}

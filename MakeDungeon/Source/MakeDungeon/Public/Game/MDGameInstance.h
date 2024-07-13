// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMDGameInstance();

	//FORCEINLINE FMDItemBaseData GetCharacterStat(int32 InLevel) const { return ItemTable.IsValidIndex(InLevel - 1) ? ItemTable[InLevel - 1] : FMDItemBaseData(); }

protected:
	//TArray<FMDItemBaseData> ItemTable;
};

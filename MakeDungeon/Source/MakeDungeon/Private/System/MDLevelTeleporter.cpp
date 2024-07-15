// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MDLevelTeleporter.h"
#include "Components/BoxComponent.h"
#include "Physics/MDCollision.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MDPlayerController.h"

AMDLevelTeleporter::AMDLevelTeleporter()
{
	PrimaryActorTick.bCanEverTick = false;
	Teleporter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Teleporter"));
	RootComponent = Teleporter;

	TeleportTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportTrigger"));
	TeleportTrigger->SetupAttachment(Teleporter);
	TeleportTrigger->SetCollisionProfileName(CPROFILE_MDTRIGGER);
	TeleportTrigger->OnComponentBeginOverlap.AddDynamic(this, 
								&AMDLevelTeleporter::OnTeleportTriggerBeginOverlap);
}

void AMDLevelTeleporter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMDLevelTeleporter::OnTeleportTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMDPlayerController* PlayerController = Cast<AMDPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->SavePlayerInfo();
	}

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage01"));
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "Character/MDCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/ObjectPoolWorldSubsystem.h"
#include "../MakeDungeon.h"

AMDProjectile::AMDProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMDProjectile::OnBeginOverlap);

	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;

	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'"));
	if (BulletMeshRef.Object)
	{
		BulletMeshComponent->SetStaticMesh(BulletMeshRef.Object);
	}
	//BulletMeshComponent->SetStaticMesh(BulletMesh.GetDefaultObject());
	BulletMeshComponent->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	BulletMeshComponent->SetupAttachment(RootComponent);
	BulletMeshComponent->CanCharacterStepUpOn = ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	OnDestroyed.AddDynamic(this, &AMDProjectile::OnDestroyedCallBack);

	//InitialLifeSpan = 3.0f;
}

void AMDProjectile::CollisionEnable()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMDProjectile::CollisionDisable()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMDProjectile::Restart(const FVector& Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->GetMaxSpeed();
	ProjectileMovement->Activate(true);
}

void AMDProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMDProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != GetOwner()) && (OtherActor != GetInstigator()))
	{
		AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(OtherActor);
		if (!MDCharacter)
		{
			
			



		}

		//UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();
		//if (ASC)
		//{
		//	//ActorLineTraceSingle
		//	//GetWorld()->OverlapMultiByChannel()
		//	//UKismetSystemLibrary::SphereOverlapActors()
		//	//UKismetSystemLibrary::SphereTraceSingle()
		//}

		CollisionDisable();

		UObjectPoolWorldSubsystem* ObjectPool = UWorld::GetSubsystem<UObjectPoolWorldSubsystem>(GetWorld());
		ObjectPool->CollectObject(this);

		MD_LOG(LogMD, Log, TEXT("Collect_Overlap"));
	}
}

void AMDProjectile::OnDestroyedCallBack(AActor* DestroyedActor)
{
	AMDProjectile* CollectActor = CastChecked<AMDProjectile>(DestroyedActor);
	UObjectPoolWorldSubsystem* ObjectPool = UWorld::GetSubsystem<UObjectPoolWorldSubsystem>(GetWorld());
	ObjectPool->CollectObject(CollectActor);

	MD_LOG(LogMD, Log, TEXT("Collect_Destroy"));
}

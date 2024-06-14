// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "Character/MDCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/ObjectPoolWorldSubsystem.h"
#include "../MakeDungeon.h"

#include "Engine/StaticMeshActor.h"

AMDProjectile::AMDProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMDProjectile::OnBeginOverlap);

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
	BulletMeshComponent->SetRelativeScale3D(FVector(0.5, 0.1, 0.1));
	BulletMeshComponent->SetupAttachment(RootComponent);
	BulletMeshComponent->CanCharacterStepUpOn = ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	ProjectileType = EProjectileType::Normal;

	bIsOverlapped = false;

	//ObjectPool Hold
	//OnDestroyed.AddDynamic(this, &AMDProjectile::OnDestroyedCallBack);

	//InitialLifeSpan = 3.0f;
}

AMDProjectile* AMDProjectile::ShootProjectile(UWorld* WorldContextObject, UClass* Class, AActor* ProjectileOwner, APawn* ProjectileInstigator, const FVector& StartPoint, const FRotator& Direction, float InitSpeed, EProjectileType Type, AActor* IgnoreActor)
{
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(StartPoint);
	SpawnTransform.SetRotation(Direction.Quaternion());

	AMDProjectile* Projectile = WorldContextObject->SpawnActorDeferred<AMDProjectile>(Class, SpawnTransform, ProjectileOwner, ProjectileInstigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->SetProjectileType(Type);
	Projectile->GetProjectileMovement()->InitialSpeed = InitSpeed;
	Projectile->SetIgnoreTarget(IgnoreActor);
	Projectile->FinishSpawning(SpawnTransform);

	return Projectile;
}

void AMDProjectile::BeginPlay()
{
	Super::BeginPlay();

	switch (ProjectileType)
	{
	case EProjectileType::Normal:
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMDProjectile::OnBeginOverlap);
		break;
	case EProjectileType::Spread:
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMDProjectile::OnBeginOverlap);
		break;
	case EProjectileType::Pierce:
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMDProjectile::OnBeginOverlap_Pierce);
		break;
	case EProjectileType::Mortar:
		
		break;
	default:
		break;
	}
	
}

void AMDProjectile::SetDead()
{
	SetActorEnableCollision(false);
	Destroy();
}

bool AMDProjectile::FindAroundTarget()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte PhysicsBody = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	TEnumAsByte Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(PhysicsBody);
	ObjectTypes.Add(Pawn);

	UClass* ClassFilter = AStaticMeshActor::StaticClass();

	TArray<AActor*> ActorToIgnore;
	TArray<AActor*> ResultActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), FVector(), 1.f, ObjectTypes, ClassFilter, ActorToIgnore, ResultActors);
	return false;
}

void AMDProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* ProjectileOwner = GetOwner();
	APawn* ProjectileInstigator = GetInstigator();
	if ((OtherActor != ProjectileOwner) && (OtherActor != ProjectileInstigator))
	{
		if(!IgnoreTarget || (IgnoreTarget && OtherActor != IgnoreTarget))
		{
			AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(OtherActor);
			if (MDCharacter)
			{
				SetDead();
			}

			//UAbilitySystemComponent* ASC = MDCharacter->GetAbilitySystemComponent();
			//if (ASC)
			//{
			//	//ActorLineTraceSingle
			//	//GetWorld()->OverlapMultiByChannel()
			//	//UKismetSystemLibrary::SphereOverlapActors()
			//	//UKismetSystemLibrary::SphereTraceSingle()
			//}

			/*CollisionDisable();

			UObjectPoolWorldSubsystem* ObjectPool = UWorld::GetSubsystem<UObjectPoolWorldSubsystem>(GetWorld());
			ObjectPool->CollectObject(this);*/

		}
	}
}

void AMDProjectile::OnBeginOverlap_Pierce(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != GetOwner()) && (OtherActor != GetInstigator()))
	{
		if (!IgnoreTarget || (IgnoreTarget && OtherActor != IgnoreTarget))
		{
			AMDCharacterBase* MDCharacter = Cast<AMDCharacterBase>(OtherActor);
			if (MDCharacter)
			{
				//SetDead();
			}

		}
	}
}

void AMDProjectile::OnDestroyedCallBack(AActor* DestroyedActor)
{
	AMDProjectile* CollectActor = CastChecked<AMDProjectile>(DestroyedActor);
	
	DestroyedActor->SetActorEnableCollision(false);
	DestroyedActor->SetActorHiddenInGame(true);

	UObjectPoolWorldSubsystem* ObjectPool = UWorld::GetSubsystem<UObjectPoolWorldSubsystem>(GetWorld());
	ObjectPool->CollectObject(CollectActor);

	MD_LOG(LogMD, Log, TEXT("Collect_Destroy"));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/MDCollision.h"
#include "Tags/MDGameplayTag.h"
#include "Item/MDWeaponBase.h"
#include "MotionWarpingComponent.h"
#include "UI/MDWidgetComponent.h"
#include "UI/MDUserWidget.h"

// Sets default values
AMDCharacterBase::AMDCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MDCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MakeDungeon/Animation/ABP_MDCharacter.ABP_MDCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	MWC = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	HpBar = CreateDefaultSubobject<UMDWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/MakeDungeon/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	TrackingSpeed = 20.f;
	bIsTrackingTarget = false;
}

void AMDCharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMDCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMDCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitWeapons();
}

void AMDCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsTrackingTarget)
	{
		FRotator TargetRotator = GetAttackDirection();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotator, DeltaSeconds, TrackingSpeed));
	}
}

UAbilitySystemComponent* AMDCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

FRotator AMDCharacterBase::GetAttackDirection() const
{
	return FRotationMatrix::MakeFromX(GetActorForwardVector()).Rotator();
}

void AMDCharacterBase::InitWeapons()
{
	bool IsFirst = true;

	for (auto& WeaponInfo : WeaponsInfo)
	{
		if (IsFirst)
		{
			CurrentWeapon = WeaponInfo.Key;
			IsFirst = false;
		}

		UMDWeaponBase* Weapon = nullptr;
		Weapon = Cast<UMDWeaponBase>(AddComponentByClass(WeaponInfo.Value, true, FTransform::Identity, false));
		if (Weapon)
		{
			Weapons.Add(WeaponInfo.Key, Weapon);
			Weapon->InitWeapon(this);
		}
	}

	//ASC->AddLooseGameplayTag(CurrentWeapon);
}

void AMDCharacterBase::OnOutOfHealth()
{
}

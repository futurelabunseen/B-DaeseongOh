// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/MDCollision.h"
#include "Tags/MDGameplayTag.h"
#include "Item/MDWeaponBase.h"
#include "MotionWarpingComponent.h"

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

	Weapon = CreateDefaultSubobject<UMDWeaponBase>(TEXT("Weapon"));

	MWC = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	TrackingSpeed = 20.f;
	bIsTrackingTarget = false;
}

void AMDCharacterBase::BeginPlay()
{
	Super::BeginPlay();

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

FGameplayTag AMDCharacterBase::GetWeaponType() const
{
	FGameplayTag CurentWeaponType = FGameplayTag();

	if (UMDWeaponAttackData* WeaponData = Weapon->GetWeaponAttackData())
	{
		CurentWeaponType = WeaponData->WeaponType;
	}

	return CurentWeaponType;
}

UAbilitySystemComponent* AMDCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

FVector AMDCharacterBase::GetAttackLocation() const
{
	return FVector();
}

FRotator AMDCharacterBase::GetAttackDirection() const
{
	return FRotationMatrix::MakeFromX(GetActorForwardVector()).Rotator();
}

void AMDCharacterBase::SwapWeapon(FGameplayTag Tag)
{
	if (MDTAG_WEAPON_TWOHANDEDSWORD == Tag)
	{
		Weapon->SetWeaponAttackData(this, LoadObject<UMDWeaponAttackData>(NULL, TEXT("/Script/MakeDungeon.MDWeaponAttackData'/Game/MakeDungeon/Data/MD_Attack_TwoHandedSword.MD_Attack_TwoHandedSword'")));
	}
	else if (MDTAG_WEAPON_BOW == Tag)
	{
		Weapon->SetWeaponAttackData(this, LoadObject<UMDWeaponAttackData>(NULL, TEXT("/Script/MakeDungeon.MDWeaponAttackData'/Game/MakeDungeon/Data/MD_Attack_Bow.MD_Attack_Bow'")));
	}
}

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
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	MWC = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	StatusBar = CreateDefaultSubobject<UMDWidgetComponent>(TEXT("Widget"));
	StatusBar->SetupAttachment(GetMesh());
	StatusBar->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> StatusBarWidgetRef(TEXT("/Game/MakeDungeon/UI/WBP_StatusBar.WBP_StatusBar_C"));
	if (StatusBarWidgetRef.Class)
	{
		StatusBar->SetWidgetClass(StatusBarWidgetRef.Class);
		StatusBar->SetWidgetSpace(EWidgetSpace::Screen);
		StatusBar->SetDrawSize(FVector2D(200.f, 30.f));
		StatusBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StatusBar->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	}

	TrackingSpeed = 20.f;
	bIsTrackingTarget = false;
	bIsCharging = false;
	bIsLooping = false;
	bIsDead = false;
}

void AMDCharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMDCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMDCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMDCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitWeapons();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const auto& Effect : PassiveEffects)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect, 0, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

void AMDCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsTrackingTarget)
	{
		FRotator TargetRotator = GetAttackDirection();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotator, DeltaSeconds, TrackingSpeed));
	}
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + GetActorForwardVector() * 100.f;
	//DrawDebugDirectionalArrow(GetWorld(), Start, End, 100.f, FColor::Blue);
}

UAbilitySystemComponent* AMDCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

FRotator AMDCharacterBase::GetAttackDirection(bool GetCursorDirection) const
{
	return FRotationMatrix::MakeFromZ(GetMesh()->GetForwardVector()).Rotator();
}

void AMDCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//SetActorEnableCollision(false);
	StatusBar->SetHiddenInGame(true);
	bIsDead = true;
}

void AMDCharacterBase::InitWeapons()
{
	bool IsFirst = true;

	for (auto& WeaponInfo : WeaponsInfo)
	{
		UMDWeaponBase* Weapon = nullptr;
		Weapon = Cast<UMDWeaponBase>(AddComponentByClass(WeaponInfo.Value, true, FTransform::Identity, false));
		if (Weapon)
		{
			Weapons.Add(WeaponInfo.Key, Weapon);
			Weapon->InitWeapon(this);
		}

		if (IsFirst)
		{
			CurrentWeapon = WeaponInfo.Key;
			IsFirst = false;
		}
	}

	//ASC->AddLooseGameplayTag(CurrentWeapon);
}

void AMDCharacterBase::OnOutOfHealth()
{
	SetDead();
}
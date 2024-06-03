// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Item/MDWeaponBase.h"
#include "Tags/MDGameplayTag.h"
#include "Player/MDPlayerController.h"
#include "Character/Abilities/AttributeSets/MDCharacterAttributeSet.h"
#include "UI/MDWidgetComponent.h"
#include "UI/MDHUDWidget.h"
#include "UI/MDCharacterStatWidget.h"
#include "../MakeDungeon.h"


AMDCharacterPlayer::AMDCharacterPlayer()
{
	ASC = nullptr;
	AttributeSet = nullptr;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("//Script/Engine.SkeletalMesh'/Game/Private/Pirate/Mesh_UE5/Full/SKM_Pirate_Full_02.SKM_Pirate_Full_02'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MakeDungeon/Animation/ABP_MDCharacter.ABP_MDCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMDPlayerState* PS = GetPlayerState<AMDPlayerState>();
	if (PS)
	{
		ASC = PS->GetAbilitySystemComponent();
		AttributeSet = PS->GetAttributeSet();
		ASC->InitAbilityActorInfo(PS, this);
		AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		
		for (const auto& StartAbility : CharacterAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : InputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			ASC->GiveAbility(StartSpec);
		}
	
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AMDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(MDTAG_WEAPON_TWOHANDEDSWORD);
}

FVector AMDCharacterPlayer::GetAttackLocation() const
{
	FHitResult HitResult;
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

	return HitResult.Location;
}

FRotator AMDCharacterPlayer::GetAttackDirection() const
{
	FHitResult HitResult;
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
	FVector MouseLocation = HitResult.Location;
	MouseLocation.Z = 0.0;

	FVector StartPoint = GetActorLocation();
	StartPoint.Z = 0.0;

	return FRotationMatrix::MakeFromX(MouseLocation - StartPoint).Rotator();
}

void AMDCharacterPlayer::SetCurrentWeapon(const FGameplayTag& Tag)
{
	if (CurrentWeapon != Tag)
	{
		CurrentWeapon = Tag;
		OnGameplayTagChanged.Broadcast();
	}
}

void AMDCharacterPlayer::StopMovement()
{
	GetController()->StopMovement();
}

void AMDCharacterPlayer::SwapWeapon(FGameplayTag Tag, UEnhancedInputLocalPlayerSubsystem* SubSysyem)
{
	if (Tag != CurrentWeapon)
	{
		if (!ASC->HasAllMatchingGameplayTags(MDTAG_WEAPON_ATTACK.GetSingleTagContainer()))
		{
			UMDWeaponBase* Weapon = Weapons.Find(Tag)->Get();
			if (Weapon)
			{
				FGameplayTagContainer CurrentOwnedTags;
				ASC->GetOwnedGameplayTags(CurrentOwnedTags);

				if (CurrentOwnedTags.HasTag(CurrentWeapon))
				{
					ASC->RemoveLooseGameplayTag(CurrentWeapon);
				}

				SubSysyem->RemoveMappingContext(Weapons[CurrentWeapon]->GetMappingContext());
				Weapons[CurrentWeapon]->SetHiddenInGame(true);
				//Off Current

				SetCurrentWeapon(Tag);

				//On New
				SubSysyem->AddMappingContext(Weapons[CurrentWeapon]->GetMappingContext(), 1);
				ASC->AddLooseGameplayTag(CurrentWeapon);
				Weapons[CurrentWeapon]->SetHiddenInGame(false);
			}
			else
			{
				MD_LOG(LogMD, Error, TEXT("No Weapon"));
			}
		}
	}
}

void AMDCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			EnableInput(PlayerController);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			SetActorEnableCollision(true);
			HpBar->SetHiddenInGame(false);
			MpBar->SetHiddenInGame(false);
			AttributeSet->Revive();
		}
	), 5.f, false);
}

void AMDCharacterPlayer::OnOutOfHealth()
{
	Super::OnOutOfHealth();
}

void AMDCharacterPlayer::EquipWeapon(FGameplayTag Tag)
{
	SetCurrentWeapon(Tag);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
			if (Subsystem)
			{
				Subsystem->AddMappingContext(Weapons[CurrentWeapon]->GetMappingContext(), 1);
				ASC->AddLooseGameplayTag(CurrentWeapon);
				Weapons[CurrentWeapon]->SetHiddenInGame(false);
			}
		}
	}
}

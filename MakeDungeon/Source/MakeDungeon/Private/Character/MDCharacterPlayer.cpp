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
#include "../MakeDungeon.h"


AMDCharacterPlayer::AMDCharacterPlayer()
{
	ASC = nullptr;
	AttributeSet = nullptr;

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

void AMDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(MDTAG_WEAPON_TWOHANDEDSWORD);
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
		//AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth)
		
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

void AMDCharacterPlayer::StopMovement()
{
	GetController()->StopMovement();
}

void AMDCharacterPlayer::SwapWeapon(FGameplayTag Tag, UEnhancedInputLocalPlayerSubsystem* SubSysyem)
{
	if (Tag != CurrentWeapon)
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

			CurrentWeapon = Tag;
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

void AMDCharacterPlayer::OnOutOfHealth()
{
	Super::OnOutOfHealth();
}

void AMDCharacterPlayer::EquipWeapon(FGameplayTag Tag)
{
	CurrentWeapon = Tag;

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

void AMDCharacterPlayer::UnequipWeapon(const FGameplayEventData* EventData)
{
	/*FGameplayAbilitySpec* SkillAbilitySpec = ASC->FindAbilitySpecFromClass(Class);
	if (SkillAbilitySpec)
	{
		ASC->ClearAbility(SkillAbilitySpec->Handle);
	}*/
}

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
#include "UI/MDCharacterStatWidget.h"
#include "UI/MDPierceCountWidget.h"
#include "Interface/MDGameInterface.h"
#include "Game/MDGameMode.h"
#include "../MakeDungeon.h"

AMDCharacterPlayer::AMDCharacterPlayer()
{
	ASC = nullptr;
	AttributeSet = nullptr;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

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
	
	PierceCount = CreateDefaultSubobject<UMDWidgetComponent>(TEXT("PierceCount"));
	PierceCount->SetupAttachment(GetMesh());
	PierceCount->SetRelativeLocation(FVector(0.f, 0.f, 280.f));
	static ConstructorHelpers::FClassFinder<UMDPierceCountWidget> PierceCountWidgetRef(TEXT("/Game/MakeDungeon/UI/WBP_Pierce.WBP_Pierce_C"));
	if (PierceCountWidgetRef.Class)
	{
		PierceCount->SetWidgetClass(PierceCountWidgetRef.Class);
		PierceCount->SetWidgetSpace(EWidgetSpace::Screen);
		PierceCount->SetDrawSize(FVector2D(50.f, 50.f));
		PierceCount->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PierceCount->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		PierceCount->SetHiddenInGame(true);
	}


	DeathCount = 3;

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
		//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
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

FRotator AMDCharacterPlayer::GetAttackDirection(bool GetCursorDirection) const
{
	FRotator ResultRotator;

	if (IsTrackingTarget() || GetCursorDirection)
	{
		FHitResult HitResult;
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
		FVector MouseLocation = HitResult.Location;
		FVector StartPoint = GetActorLocation();

		MouseLocation.Z = StartPoint.Z;

		ResultRotator = FRotationMatrix::MakeFromX(MouseLocation - StartPoint).Rotator();
	}
	else
	{
		ResultRotator = Super::GetAttackDirection(GetCursorDirection);
	}

	return ResultRotator;
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

void AMDCharacterPlayer::SwapWeapon(FGameplayTag Tag, UEnhancedInputLocalPlayerSubsystem* SubSystem)
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

				SubSystem->RemoveMappingContext(Weapons[CurrentWeapon]->GetMappingContext());
				Weapons[CurrentWeapon]->SetHiddenInGame(true);
				//Off Current

				SetCurrentWeapon(Tag);

				//On New
				SubSystem->AddMappingContext(Weapons[CurrentWeapon]->GetMappingContext(), 1);
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

void AMDCharacterPlayer::SetPierceCount(int32 Value)
{
	UMDPierceCountWidget* PierceCountWidget = Cast<UMDPierceCountWidget>(PierceCount->GetWidget());
	if (PierceCountWidget)
	{
		PierceCountWidget->SetCount(Value);
	}
}

void AMDCharacterPlayer::SetVisiblePierceCount(bool IsVisible)
{
	PierceCount->SetHiddenInGame(!IsVisible);
}

void AMDCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);

		--DeathCount;
		if(0 >= DeathCount)
		{
			IMDGameInterface* MDGamemode = Cast<IMDGameInterface>(GetWorld()->GetAuthGameMode());
			if (MDGamemode)
			{
				MDGamemode->OnPlayerDead();
			}
		}
		else
		{
			FTimerHandle DeadTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
				[&, PlayerController]()
				{
					PlayerController->EnableInput(PlayerController);
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					SetActorEnableCollision(true);
					StatusBar->SetHiddenInGame(false);
					AttributeSet->Revive();
				}
			), 5.f, false);
		}
	}
}

void AMDCharacterPlayer::OnOutOfHealth()
{
	Super::OnOutOfHealth();
}

void AMDCharacterPlayer::EquipWeapon(FGameplayTag Tag)
{
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

				OnGameplayTagChanged.Broadcast();
			}
		}
	}
}

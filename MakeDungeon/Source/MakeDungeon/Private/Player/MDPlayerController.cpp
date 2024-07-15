// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Data/MDInputData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Tags/MDGameplayTag.h"
#include "Character/MDCharacterPlayer.h"
#include "UI/MDHUDWidget.h"
#include "UI/MDGameScoreWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../MakeDungeon.h"

#include "Game/MDGameInstance.h"
#include "GameData/MDItemBaseData.h"
#include "UI/MDInventory.h"

//#include "Item/MDWeaponBase.h"

AMDPlayerController::AMDPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMDHUDWidget> MDHUDWidgetRef(TEXT(
								"/Game/MakeDungeon/UI/WBP_MDHUD.WBP_MDHUD_C"));
	if(MDHUDWidgetRef.Class)
	{
		MDHUDWidgetClass = MDHUDWidgetRef.Class;
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AMDPlayerController::GameScoreChanged(int32 NewScore)
{
	if(MDGameScoreWidget)
	{
		MDGameScoreWidget->SetScore(NewScore);
	}
}

void AMDPlayerController::GameClear()
{
	if (ClearWidgetClass)
	{
		OverWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::SetGamePaused(this, true);
	}
}

void AMDPlayerController::GameOver()
{
	if (OverWidgetClass)
	{
		OverWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::SetGamePaused(this, true);
	}
}

void AMDPlayerController::SpawnTimeChanged(int32 NewTime)
{
	if (MDGameScoreWidget)
	{
		MDGameScoreWidget->SetSpawnTime(NewTime);
	}
}

UMDUserWidgetBase* AMDPlayerController::GetUI(EMDUIType UIType)
{
	if (MDHUDWidget)
	{
		return MDHUDWidget->GetUI(UIType);
	}

	return nullptr;
}

void AMDPlayerController::VisibleUI(EMDUIType UIType)
{
	if(MDHUDWidget)
	{
		MDHUDWidget->VisibleUI(UIType);
	}
}

void AMDPlayerController::HideUI(EMDUIType UIType)
{
	if(MDHUDWidget)
	{
		MDHUDWidget->HideUI(UIType);
	}
}

void AMDPlayerController::SavePlayerInfo()
{
	if (MDHUDWidget)
	{
		UMDInventory* PlayerInventory = Cast<UMDInventory>(MDHUDWidget->GetUI(EMDUIType::HUD_Inventory));
		if (PlayerInventory)
		{
			TArray<UObject*> PlayerAllItems = PlayerInventory->GetAllItems();

			UMDGameInstance* MDGameInstance = Cast<UMDGameInstance>(GetGameInstance());

			if (MDGameInstance)
			{
				for (auto Item : PlayerAllItems)
				{
					UMDItemBaseData* NewItem = DuplicateObject<UMDItemBaseData>(Cast<UMDItemBaseData>(Item), MDGameInstance);

					MDGameInstance->PlayerAllItems.Add(NewItem);
				}
			}
		}
	}
}

void AMDPlayerController::LoadPlayerInfo()
{
	if (MDHUDWidget)
	{
		UMDGameInstance* MDGameInstance = Cast<UMDGameInstance>(GetGameInstance());
		UMDInventory* PlayerInventory = Cast<UMDInventory>(MDHUDWidget->GetUI(EMDUIType::HUD_Inventory));

		if (MDGameInstance && PlayerInventory)
		{
			for (auto Item : MDGameInstance->PlayerAllItems)
			{
				PlayerInventory->AddItem(Item);
			}

			MDGameInstance->PlayerAllItems.Empty();
		}
	}
}

void AMDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(!IsMatchLevel(TEXT("Logo")))
	{
		CreateHUD();
	}

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(InputData->DefaultMappingContext, 0);
	}
}

void AMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(InputData->KeyboardMoveAction, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::KeyboardMove);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Started, 
									this, &AMDPlayerController::OnMouseMoveStarted);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::OnMouseMoveTriggered);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Canceled, 
									this, &AMDPlayerController::OnMouseMoveReleased);
		EnhancedInputComponent->BindAction(InputData->MouseMoveAction, ETriggerEvent::Completed, 
									this, &AMDPlayerController::OnMouseMoveReleased);
		EnhancedInputComponent->BindAction(InputData->CameraMoveAction, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::OnCameraMove);
		EnhancedInputComponent->BindAction(InputData->CameraMoveAction, ETriggerEvent::Completed, 
									this, &AMDPlayerController::OffCameraMove);
		EnhancedInputComponent->BindAction(InputData->CameraRotateAction, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::OnCameraRotate);
		EnhancedInputComponent->BindAction(InputData->CameraZoomAction, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::OnCameraZoom);

		EnhancedInputComponent->BindAction(InputData->InteractionAction, ETriggerEvent::Started, 
									this, &AMDPlayerController::Interaction);
		EnhancedInputComponent->BindAction(InputData->WeaponSwapAction, ETriggerEvent::Started, 
									this, &AMDPlayerController::SwapWeapon);
		EnhancedInputComponent->BindAction(InputData->VisibleInventoryAction, ETriggerEvent::Started,
									this, &AMDPlayerController::VisibleUI, EMDUIType::HUD_Inventory);
		
		EnhancedInputComponent->BindAction(InputData->DebugAction_AddItem, ETriggerEvent::Started,
									this, &AMDPlayerController::AddItem_Test);

		EnhancedInputComponent->BindAction(InputData->JumpAction, ETriggerEvent::Started,
									this, &AMDPlayerController::ResetGame);

		EnhancedInputComponent->BindAction(InputData->AttackAction, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_PRIMARYATTACK);
		EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_SKILL_01);
		EnhancedInputComponent->BindAction(InputData->SkillAction_01, ETriggerEvent::Completed, 
									this, &AMDPlayerController::GASInputReleased, MDTAG_WEAPON_SKILL_01);
		EnhancedInputComponent->BindAction(InputData->SkillAction_02, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_SKILL_02);
		EnhancedInputComponent->BindAction(InputData->SkillAction_03, ETriggerEvent::Triggered, 
									this, &AMDPlayerController::GASInputPressed, MDTAG_WEAPON_SKILL_03);
		EnhancedInputComponent->BindAction(InputData->SkillAction_03, ETriggerEvent::Completed, 
									this, &AMDPlayerController::GASInputReleased, MDTAG_WEAPON_SKILL_03);
	}
}

void AMDPlayerController::CreateHUD()
{
	MDHUDWidget = CreateWidget<UMDHUDWidget>(this, MDHUDWidgetClass);
	if (MDHUDWidget)
	{
		MDHUDWidget->AddToViewport();
		
		MDGameScoreWidget = Cast<UMDGameScoreWidget>(MDHUDWidget->GetUI(EMDUIType::HUD_GameScore));
	}

	LoadPlayerInfo();

	/*for (const auto& HUDWidgetClass : HUDWidgetClasses)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass.Value);

		if (NewWidget)
		{
			NewWidget->AddToViewport();

			if (EMDUIType::HUD_GameScore == HUDWidgetClass.Key)
			{

			}

			HUDWidgets.Add(HUDWidgetClass.Key, NewWidget);
		}

	}*/

}

void AMDPlayerController::KeyboardMove(const FInputActionValue& Value)
{
	StopMovement();

	FVector2D MovementVector = Value.Get<FVector2D>();

	const float NomalizeSize = 1.f;
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = NomalizeSize;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > NomalizeSize)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = NomalizeSize;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	GetPawn()->AddMovementInput(MoveDirection, MovementVectorSize);
}

void AMDPlayerController::OnMouseMoveStarted()
{
	StopMovement();
}

void AMDPlayerController::OnMouseMoveTriggered()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->
										GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void AMDPlayerController::OnMouseMoveReleased()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination);
}

void AMDPlayerController::OnCameraMove(const FInputActionValue& Value)
{
	bIsCameraMove = true;
}

void AMDPlayerController::OffCameraMove(const FInputActionValue& Value)
{
	bIsCameraMove = false;
}

void AMDPlayerController::OnCameraRotate(const FInputActionValue& Value)
{
	if(bIsCameraMove)
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		AMDCharacterPlayer* MDPlayer = CastChecked<AMDCharacterPlayer>(GetPawn());
		USpringArmComponent* PlayerCameraBoom = MDPlayer->GetCameraBoom();
		FRotator CurrentRotaion = PlayerCameraBoom->GetRelativeRotation();
		PlayerCameraBoom->SetRelativeRotation(FRotator(FMath::Clamp(CurrentRotaion.Pitch + LookAxisVector.Y, -70.0, -10.0),
			CurrentRotaion.Yaw + LookAxisVector.X, 0.0));
	}
}

void AMDPlayerController::OnCameraZoom(const FInputActionValue& Value)
{
	AMDCharacterPlayer* MDPlayer = CastChecked<AMDCharacterPlayer>(GetPawn());
	USpringArmComponent* PlayerCameraBoom = MDPlayer->GetCameraBoom();
	PlayerCameraBoom->TargetArmLength = FMath::Clamp(PlayerCameraBoom->TargetArmLength + Value.GetMagnitude() * 10.f, 200.f, 1000.f);
}

void AMDPlayerController::ResetGame()
{
	AGameMode* GM = Cast<AGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RestartGame();
	}
}

void AMDPlayerController::Interaction()
{
	OnInteraction.Broadcast();
}

void AMDPlayerController::SwapWeapon()
{
	AMDCharacterPlayer* MDPlayer = CastChecked<AMDCharacterPlayer>(GetPawn	());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (MDTAG_WEAPON_TWOHANDEDSWORD == MDPlayer->GetWeaponType() && SubSystem)
	{
		MDPlayer->SwapWeapon(MDTAG_WEAPON_BOW, SubSystem);
	}
	else
	{
		MDPlayer->SwapWeapon(MDTAG_WEAPON_TWOHANDEDSWORD, SubSystem);
	}
}

void AMDPlayerController::AddItem_Test()
{
	//MDHUDWidget->AddItem(EMDUIType::HUD_Inventory);
}

void AMDPlayerController::GASInputStarted(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();

	ASC->TryActivateAbilitiesByTag(Tag.GetSingleTagContainer());
}

void AMDPlayerController::GASInputPressed(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();
	
	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			if (Spec.IsActive())
			{
				ASC->AbilitySpecInputPressed(Spec);
				MD_LOG(LogMD, Log, TEXT("Pressed"));
			}
			else
			{
				ASC->TryActivateAbility(Spec.Handle);
				MD_LOG(LogMD, Log, TEXT("Activate"));
			}
		}
	}

}

void AMDPlayerController::GASInputReleased(FGameplayTag Tag)
{
	UAbilitySystemComponent* ASC = GetPlayerState<AMDPlayerState>()->GetAbilitySystemComponent();

	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();

	for (auto& Spec : ActivatableAbilities)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			if (Spec.IsActive())
			{
				ASC->AbilitySpecInputReleased(Spec);
				MD_LOG(LogMD, Log, TEXT("Released"));
			}
		}
	}
}

bool AMDPlayerController::IsMatchLevel(const FString& InLevelName)
{
	FString LevelNameWithPath = GetWorld()->GetMapName();
	int32 LastUnderbarIndex;
	LevelNameWithPath.FindLastChar('_', LastUnderbarIndex);
	FString LevelName = LevelNameWithPath.Mid(LastUnderbarIndex + 1);
	
	return LevelName.Equals(InLevelName);
}

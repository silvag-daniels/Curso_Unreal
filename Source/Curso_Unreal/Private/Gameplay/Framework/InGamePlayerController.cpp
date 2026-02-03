// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Framework/InGamePlayerController.h"
#include "Gameplay/Inventory/InventoryComp.h"
#include "Gameplay/Inventory/InventoryItem.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputConfig);

	auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	InputSubsystem->AddMappingContext(InputConfig->InGameMappingContext, 0);
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent);

	Input->BindAction(InputConfig->SetDestinationClick, ETriggerEvent::Started, this, &AInGamePlayerController::OnMovementRequestStarted);
	Input->BindAction(InputConfig->SetDestinationClick, ETriggerEvent::Triggered, this, &AInGamePlayerController::OnMovementRequestTriggered);
	Input->BindAction(InputConfig->SetDestinationClick, ETriggerEvent::Completed, this, &AInGamePlayerController::OnMovementRequestCompleted);
	Input->BindAction(InputConfig->SetDestinationClick, ETriggerEvent::Canceled, this, &AInGamePlayerController::OnMovementRequestCompleted);
	Input->BindAction(InputConfig->SetOpenInventory, ETriggerEvent::Started, this, &AInGamePlayerController::OnSetOpenInventoryClicked);
	
}

void AInGamePlayerController::OnMovementRequestStarted()
{
	StopMovement();
	FollowTime = 0.f;
}

void AInGamePlayerController::OnMovementRequestTriggered()
{
	FHitResult HitResult;

	FollowTime += GetWorld()->GetDeltaSeconds();

	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}


	FVector Destination = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
	GetPawn()->AddMovementInput(Destination);
}

void AInGamePlayerController::OnMovementRequestCompleted()
{
	if (FollowTime <= FollowTresholdTime)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	}	
}

void AInGamePlayerController::OnSetOpenInventoryClicked()
{
	UInventoryComp* Inventory = GetPawn()->FindComponentByClass<UInventoryComp>();
	OnOpenInventory.Broadcast(Inventory);
}

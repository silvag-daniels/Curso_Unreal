// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Framework/InGamePlayerController.h"
#include "Gameplay/Inventory/InventoryComp.h"
#include "Gameplay/Inventory/InventoryItem.h"
#include "Gameplay/Combat/CombatComp.h"
#include "Gameplay/Combat/TargetingComp.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"


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

void AInGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	UTargetingComp* TargetingComp = nullptr;

	if (HitResult.bBlockingHit)
	{
		TargetingComp = HitResult.GetActor()->FindComponentByClass<UTargetingComp>();
	}

	UCombatComp * CombatComp = GetPawn()->FindComponentByClass<UCombatComp>();
	check(CombatComp);
	CombatComp->SetTarget(TargetingComp);

}

void AInGamePlayerController::OnMovementRequestStarted()
{
	/*StopMovement();
	FollowTime = 0.f;*/

	UAbilitySystemComponent* ASC = GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	FGameplayTagContainer GameplayTags(FGameplayTag::RequestGameplayTag("Abilities.Movement.Base"));
	
	if (ASC)
	{
		FGameplayEventData EventData;
		EventData.Instigator = GetPawn();
		UCombatComp* CombatComp = GetPawn()->FindComponentByClass<UCombatComp>();
		EventData.OptionalObject = CombatComp->GetTarget();
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Abilities.Movement.Base");
		ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void AInGamePlayerController::OnMovementRequestTriggered()
{
	/*FHitResult HitResult;

	FollowTime += GetWorld()->GetDeltaSeconds();

	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}


	FVector Destination = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
	GetPawn()->AddMovementInput(Destination);*/
}

void AInGamePlayerController::OnMovementRequestCompleted()
{
	UAbilitySystemComponent* ASC = GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Abilities.Movement.NotifyInputFinished");
		ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void AInGamePlayerController::OnSetOpenInventoryClicked()
{
	/*UInventoryComp* Inventory = GetPawn()->FindComponentByClass<UInventoryComp>();
	OnOpenInventory.Broadcast(Inventory);*/
}

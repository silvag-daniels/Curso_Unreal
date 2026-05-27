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
	Input->BindAction(InputConfig->SetDestinationClick, ETriggerEvent::Completed, this, &AInGamePlayerController::OnMovementRequestCompleted);
	Input->BindAction(InputConfig->SetDestinationClick, ETriggerEvent::Canceled, this, &AInGamePlayerController::OnMovementRequestCompleted);
	Input->BindAction(InputConfig->SetThrowBomb, ETriggerEvent::Started, this, &AInGamePlayerController::OnSetThrowBombClicked);
	
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
	if (bFreezePlayer)
	{
		return;
	}

	Target = nullptr;

	UAbilitySystemComponent* ASC = GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	FGameplayTagContainer GameplayTags(FGameplayTag::RequestGameplayTag("Abilities.Movement.Base"));
	
	if (ASC)
	{
		FGameplayEventData EventData;
		EventData.Instigator = GetPawn();
		UCombatComp* CombatComp = GetPawn()->FindComponentByClass<UCombatComp>();
		EventData.OptionalObject = CombatComp->GetTarget();
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Abilities.Movement.Base");

		for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
		{
			if (Spec.Ability && Spec.Ability->AbilityTags.HasTagExact(FGameplayTag::RequestGameplayTag("Abilities.Movement.Base")))
			{
				ASC->CancelAbilityHandle(Spec.Handle);
			}
		}

		ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void AInGamePlayerController::OnMovementRequestCompleted()
{
	UCombatComp* CombatComp = GetPawn()->FindComponentByClass<UCombatComp>();
	Target = CombatComp->GetTarget();

	UAbilitySystemComponent* ASC = GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Abilities.Movement.NotifyInputFinished");
		ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}

}

void AInGamePlayerController::NotifyDestinationReached(const FGameplayTag& AbilityTag, bool bEndAbility)
{
	UAbilitySystemComponent* ASC = GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	if (bEndAbility)
	{
		for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
		{
			if (Spec.Ability && Spec.Ability->AbilityTags.HasTagExact(AbilityTag))
			{
				ASC->CancelAbilityHandle(Spec.Handle);
			}
		}
	}

	FGameplayTagContainer AttackTags(FGameplayTag::RequestGameplayTag("Abilities.Attack.Base"));
	ASC->TryActivateAbilitiesByTag(AttackTags);
}

void AInGamePlayerController::NotifyChangeFreezeCharacter()
{
	bFreezePlayer = !bFreezePlayer;
}

void AInGamePlayerController::OnSetThrowBombClicked()
{
	if (bFreezePlayer)
	{
		return;
	}
	UAbilitySystemComponent* ASC = GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Abilities")))
		{
			ASC->CancelAbilityHandle(Spec.Handle);
		}
	}

	FGameplayTagContainer AttackTags(FGameplayTag::RequestGameplayTag("Abilities.Attack.ThrowBomb"));
	ASC->TryActivateAbilitiesByTag(AttackTags);
}
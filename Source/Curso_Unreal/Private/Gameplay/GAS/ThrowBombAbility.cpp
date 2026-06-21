// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/ThrowBombAbility.h"
#include "GameFramework/Character.h"
#include "Gameplay/Framework/InGamePlayerController.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GAS/CharacterAttributeSet.h"
#include "Gameplay/Inventory/InventoryComp.h"
#include "Gameplay/Inventory/InventoryItem.h"

UThrowBombAbility::UThrowBombAbility()
{
	//Add tag
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Attack.ThrowBomb")));
}

void UThrowBombAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	AInGamePlayerController* PlayerController = Cast<AInGamePlayerController>(Character->GetController());

	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Attack.ThrowBomb.AnimationFinished"));
	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag, nullptr, true);

	if (WaitGameplayEventTask)
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UThrowBombAbility::OnGameplayEventReceived);
		WaitGameplayEventTask->ReadyForActivation();
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UThrowBombAbility::OnGameplayEventReceived(const FGameplayEventData Payload)
{
	//Spawn bomb and throw it
	ACharacter* Character = Cast<ACharacter>(GetCurrentActorInfo()->AvatarActor.Get());

	FVector SpawnLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = Character->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	SpawnParams.Instigator = Character;

	AActor* Bomb = GetWorld()->SpawnActor<AActor>(BombClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (!Bomb)
	{
		return;
	}

	UPrimitiveComponent* BombMesh = Cast<UPrimitiveComponent>(Bomb->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (BombMesh)
	{
		UAbilitySystemComponent* ASC = Character->GetComponentByClass<UAbilitySystemComponent>();
		float ThrowStrength = ASC->GetNumericAttribute(UCharacterAttributeSet::GetThrowStrengthAttribute());
		float ThrowUpwardForce = ASC->GetNumericAttribute(UCharacterAttributeSet::GetThrowUpwardForceAttribute());
		FVector ThrowDirection = Character->GetActorForwardVector() * ThrowStrength + FVector(0, 0, ThrowUpwardForce);
		BombMesh->AddImpulse(ThrowDirection, NAME_None, true);
		UInventoryComp* InventoryComp = Character->GetComponentByClass<UInventoryComp>();
		InventoryComp->RemoveItem(BombItem, 1);
	}
}

bool UThrowBombAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return false;
	}

	if (!BombClass)
	{
		return false;
	}

	if (!BombItem)
	{
		return false;
	}

	UInventoryComp* InventoryComp = Character->GetComponentByClass<UInventoryComp>();
	if (!InventoryComp)
	{
		return false;
	}

	if (InventoryComp->GetItemQuantity(BombItem) < 1)
	{
		return false;
	}

	return true;
}

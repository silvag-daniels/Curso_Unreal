// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/AnimNotifySendGameplayEvent.h"
#include "AbilitySystemComponent.h"

UAnimNotifySendGameplayEvent::UAnimNotifySendGameplayEvent()
{
}

void UAnimNotifySendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemComponent* ASC = MeshComp->GetOwner()->GetComponentByClass<UAbilitySystemComponent>();

	if (ASC)
	{
		FGameplayEventData data;
		ASC->HandleGameplayEvent(GameplayEventTag, &data);
	}
}

FString UAnimNotifySendGameplayEvent::GetNotifyName_Implementation() const
{
	FString DisplayName = TEXT("GameplayEvent");

	if (GameplayEventTag.IsValid())
	{
		DisplayName = GameplayEventTag.ToString();
		DisplayName += TEXT(" (Burst)");
	}

	return DisplayName;
}

#if WITH_EDITOR
bool UAnimNotifySendGameplayEvent::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif

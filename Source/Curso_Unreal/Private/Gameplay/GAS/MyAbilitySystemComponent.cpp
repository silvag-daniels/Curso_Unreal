// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/MyAbilitySystemComponent.h"

void UMyAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	GrantInitialAbilities();
}

void UMyAbilitySystemComponent::GrantInitialAbilities()
{
	for(TSubclassOf<UGameplayAbility> Ability : InitialAbilities)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability, 1, INDEX_NONE, this);
			GiveAbility(AbilitySpec);
		}
	}
}

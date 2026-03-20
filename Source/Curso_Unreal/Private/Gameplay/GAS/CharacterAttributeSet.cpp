// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/CharacterAttributeSet.h"
#include "Gameplay/GAS/MyAbilitySystemComponent.h"

void UCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributeChange(Attribute, NewValue);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributeChange(Attribute, NewValue);
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	const FString& AttributeName = Attribute.GetName();
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponentChecked();
	CastChecked<UMyAbilitySystemComponent>(ASC)->OnAttributeValueChanged.Broadcast(ASC->GetOwner(), AttributeName, OldValue, NewValue);
}

void UCharacterAttributeSet::ClampAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	FGameplayAttribute ManaActual = GetManaCurrentAttribute();

	if (Attribute == ManaActual)
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetManaMax());
	}

}

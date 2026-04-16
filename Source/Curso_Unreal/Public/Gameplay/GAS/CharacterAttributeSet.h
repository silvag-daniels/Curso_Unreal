// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class CURSO_UNREAL_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HealthMax);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HealthCurrent;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HealthCurrent);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ManaMax;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ManaMax);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ManaCurrent;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ManaCurrent);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ThrowStrength;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ThrowStrength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ThrowUpwardForce;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ThrowUpwardForce);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void ClampAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) const;



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeValueChanged, AActor*, AttributeOwner, const FString&, AttributeName, float, OldValue, float, NewValue);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CURSO_UNREAL_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;


	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeValueChanged OnAttributeValueChanged;

private:
	void GrantInitialAbilities();
};

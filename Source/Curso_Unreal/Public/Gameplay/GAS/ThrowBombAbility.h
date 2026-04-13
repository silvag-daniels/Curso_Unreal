// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ThrowBombAbility.generated.h"

/**
 * 
 */
UCLASS()
class CURSO_UNREAL_API UThrowBombAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnGameplayEventReceived(const FGameplayEventData Payload);

	class UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask;

public:
	UThrowBombAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BombClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInventoryItemDefinition* BombItem;
	
};

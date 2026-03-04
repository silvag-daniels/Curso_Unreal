// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "BaseMovementAbility.generated.h"

/**
 * 
 */
UCLASS()
class CURSO_UNREAL_API UBaseMovementAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBaseMovementAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnEventNotifyInputReleased(FGameplayEventData Payload);
	void NotiifyInputReleased();

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	TObjectPtr<UBaseMovementAbilityTask> BaseMovementTask;

private:
	UFUNCTION()
	void FinishAbility();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityFinished);

UCLASS()
class CURSO_UNREAL_API UBaseMovementAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UBaseMovementAbilityTask();
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Create base movement task"))
	static UBaseMovementAbilityTask* CreateBaseMovementTask(UGameplayAbility* OwningAbility, float TresholdTime = 0.2f);

	void NotifyInputReleased();

	UPROPERTY(BlueprintAssignable, Category = "Ability|Tasks")
	FOnAbilityFinished OnAbilityFinished;

protected:
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityEnded) override;

	float TresholdTime = 0.0f;
	float FollowTime = 0.0f;
	bool bIsPerformingSMTL = false;
	FVector Destination = FVector::ZeroVector;
	TWeakObjectPtr<ACharacter> MyCharacter;
	TWeakObjectPtr<APlayerController> MyPlayerController;
};
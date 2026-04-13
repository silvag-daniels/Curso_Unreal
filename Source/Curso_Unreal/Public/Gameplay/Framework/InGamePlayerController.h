// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

// Forward declaration
class UInputMappingContext;
class UInputAction;

UCLASS()
class CURSO_UNREAL_API UInGameInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInputMappingContext> InGameMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInputAction> SetDestinationClick;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInputAction> SetDestinationTouch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInputAction> SetOpenInventory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInputAction> SetThrowBomb;
};

/**
 * 
 */

class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenInvetory, UInventoryComp*, Items);

UCLASS()
class CURSO_UNREAL_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInGameInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Framework")
	float FollowTresholdTime = 0.5f;

	UPROPERTY(BlueprintAssignable)
	FOnOpenInvetory OnOpenInventory;

protected:
	FVector CachedDestination;
	float FollowTime = 0.0f;
	TObjectPtr<class UTargetingComp> Target = nullptr;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	void OnMovementRequestStarted();
	void OnMovementRequestCompleted();
	void OnSetOpenInventoryClicked();
	void OnSetThrowBombClicked();

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
	UTargetingComp* GetTarget() const { return Target; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void NotifyDestinationReached(const FGameplayTag& AbilityTag, bool bEndAbility = true);


};

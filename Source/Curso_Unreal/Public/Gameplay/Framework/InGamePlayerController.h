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
};

/**
 * 
 */
UCLASS()
class CURSO_UNREAL_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Framework")
	TObjectPtr<UInGameInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Framework")
	float FollowTresholdTime = 0.5f;

protected:
	FVector CachedDestination;
	float FollowTime = 0.0f;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnMovementRequestStarted();
	void OnMovementRequestTriggered();
	void OnMovementRequestCompleted();


};

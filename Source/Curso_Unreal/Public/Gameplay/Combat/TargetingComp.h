// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetStateChanged, AActor*, TargetSource);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UTargetingComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComp();

	UPROPERTY(BlueprintAssignable, Category = "Target")
	FOnTargetStateChanged OnTargeted;

	UPROPERTY(BlueprintAssignable, Category = "Target")
	FOnTargetStateChanged OnUntargeted;

	UFUNCTION(BlueprintCallable, Category = "Target")
	void StartBeingTargeted(AActor* TargetSource);

	UFUNCTION(BlueprintCallable, Category = "Target")
	void StopBeingTargeted(AActor* TargetSource);
};

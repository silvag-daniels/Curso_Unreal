// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComp.generated.h"

class UTargetingComp;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UCombatComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComp();

protected:
	TWeakObjectPtr<UTargetingComp> Target;

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetTarget(UTargetingComp* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UTargetingComp* GetTarget() const { return Target.Get(); }



};

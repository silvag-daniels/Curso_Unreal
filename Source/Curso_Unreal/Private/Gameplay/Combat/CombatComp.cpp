// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Combat/CombatComp.h"
#include "Gameplay/Combat/TargetingComp.h"

// Sets default values for this component's properties
UCombatComp::UCombatComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCombatComp::SetTarget(UTargetingComp* NewTarget)
{
	if (NewTarget != Target)
	{
		if(Target.IsValid())
		{
			Target->StopBeingTargeted(GetOwner());
		}

		Target = NewTarget;

		if(Target.IsValid())
		{
			Target->StartBeingTargeted(GetOwner());
		}
	}
		
}


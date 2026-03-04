// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Combat/TargetingComp.h"

// Sets default values for this component's properties
UTargetingComp::UTargetingComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTargetingComp::StartBeingTargeted(AActor* TargetSource)
{
	OnTargeted.Broadcast(TargetSource);
}

void UTargetingComp::StopBeingTargeted(AActor* TargetSource)
{
	OnUntargeted.Broadcast(TargetSource);
}


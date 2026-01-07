// Fill out your copyright notice in the Description page of Project Settings.

#include "Movement/MyMovementComp.h"

// Sets default values for this component's properties
UMyMovementComp::UMyMovementComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyMovementComp::BeginPlay()
{
	Super::BeginPlay();

	NextLocation = GetOwner()->GetActorLocation();
	GetOwner()->SetActorTickEnabled(false);
	
}


void UMyMovementComp::MoveToLocation(const FVector& TargetLocation)
{
	NextLocation = TargetLocation;
	GetOwner()->SetActorTickEnabled(true);
}

// Called every frame
void UMyMovementComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector Direction = (NextLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;
	if (FVector::Dist(NewLocation, NextLocation) < (MovementSpeed * DeltaTime))
	{
		NewLocation = NextLocation;
		GetOwner()->SetActorTickEnabled(false);
	}
	GetOwner()->SetActorLocation(NewLocation);

}


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
	SetComponentTickEnabled(false);
	
}


void UMyMovementComp::MoveToLocation(const FVector& TargetLocation)
{
	if(!OnArrivalToDestination.IsAlreadyBound(this, &UMyMovementComp::OnArrival))
	{
		OnArrivalToDestination.AddDynamic(this, &UMyMovementComp::OnArrival);
	}

	NextLocation = TargetLocation;
	SetComponentTickEnabled(true);
}

void UMyMovementComp::PrintLocationHistory() const
{
	for (const FVector& Location : LocationHistory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Location.ToString());
	}
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
		LocationHistory.Emplace(NewLocation);
		SetComponentTickEnabled(false);
		OnArrivalToDestination.Broadcast(NewLocation);
		OnArrivalToDestination.RemoveDynamic(this, &UMyMovementComp::OnArrival);
	}
	GetOwner()->SetActorLocation(NewLocation);

}

void UMyMovementComp::OnArrival(const FVector& destination)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "XDXDXD");
}


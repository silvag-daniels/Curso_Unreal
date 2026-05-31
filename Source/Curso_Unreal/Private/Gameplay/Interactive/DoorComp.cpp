// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Interactive/DoorComp.h"

// Sets default values for this component's properties
UDoorComp::UDoorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UDoorComp::AddLock(uint8 Keys)
{
	RequiredKeys += Keys;
}

void UDoorComp::RemoveLock(uint8 Keys)
{
	RequiredKeys -= Keys;
	if (RequiredKeys == 0)
	{
		OnLockUnlocked.Broadcast(this);
	}
}

bool UDoorComp::IsLocked() const
{
	return RequiredKeys > 0;
}

void UDoorComp::ForceUnlock()
{
	RequiredKeys = 0;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Interactive/KeyComp.h"
#include "Gameplay/Interactive/DoorComp.h"

// Sets default values for this component's properties
void UKeyComp::BeginPlay()
{
	Super::BeginPlay();

	UDoorComp* door = GetTargetDoor();
	if (door)
	{
		door->AddLock(1);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No door found");
	}
}

UKeyComp::UKeyComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UDoorComp* UKeyComp::GetTargetDoor() const
{
	if (TargetDoor.IsValid())
	{
		return TargetDoor.Get();
	}
	if (TargetDoorActor.IsValid())
	{
		TargetDoor = TargetDoorActor->GetComponentByClass<UDoorComp>();
		if (TargetDoor.IsValid())
		{
			return TargetDoor.Get();
		}
	}
	return nullptr;
}

void UKeyComp::SetTargetDoor(UDoorComp* NewTargetDoor)
{
	TargetDoor = NewTargetDoor;
}

bool UKeyComp::UseKey()
{
	if (TargetDoor.IsValid())
	{
		TargetDoor->RemoveLock();
		return true;
	}

	return false;
}


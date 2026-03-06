// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Interactive/PickableComp.h"
#include "Components/ShapeComponent.h"

// Sets default values for this component's properties
UPickableComp::UPickableComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPickableComp::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoRegisterToCollision)
	{
		TArray<UShapeComponent*> shapes;
		GetOwner()->GetComponents<UShapeComponent>(shapes);

		for (UShapeComponent* shape : shapes)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &UPickableComp::OnShapeOverlap);
		}
	}
	
}

void UPickableComp::PickUp(AActor* Instigator)
{
	OnPickedUp.Broadcast(Instigator);
	if (bDestroyOnPick && ValidPicker)
	{
		GetOwner()->Destroy();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,"Pickup Failed");
	}
}

void UPickableComp::OnShapeOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(OtherActor);
}


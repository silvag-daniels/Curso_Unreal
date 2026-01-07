// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyMovementComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UMyMovementComp : public UActorComponent
{
	GENERATED_BODY()

private:
	FVector NextLocation;

public:	
	// Sets default values for this component's properties
	UMyMovementComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MovementSpeed = 100.f;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveToLocation(const FVector& TargetLocation);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

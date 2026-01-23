// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyMovementComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrivalToDestination, const FVector&, destination);

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

	UFUNCTION()
	void OnArrival(const FVector& destination);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100.f;

	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnArrivalToDestination OnArrivalToDestination;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToLocation(const FVector& TargetLocation);

	void PrintLocationHistory() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FVector> LocationHistory;

};

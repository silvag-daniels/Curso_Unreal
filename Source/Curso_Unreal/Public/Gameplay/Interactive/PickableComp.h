// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickableComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickedUpSignature, AActor*, PickerUpper);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UPickableComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickableComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnShapeOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(BlueprintAssignable, Category = "Gameplay | Interaction")
	FOnPickedUpSignature OnPickedUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay | Interaction")
	bool bAutoRegisterToCollision = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay | Interaction")
	bool bDestroyOnPick = true;

	UFUNCTION(BlueprintCallable, Category = "Gameplay | Interaction")
	void PickUp(AActor* Instigator);

		
};

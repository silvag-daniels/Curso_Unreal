// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyComp.generated.h"

class UDoorComp;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UKeyComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UKeyComp();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Key")
	TWeakObjectPtr<AActor> TargetDoorActor;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	mutable TWeakObjectPtr<UDoorComp> TargetDoor;

public:
	UFUNCTION(BlueprintCallable, Category = "Key")
	UDoorComp* GetTargetDoor() const;

	UFUNCTION(BlueprintCallable, Category = "Key")
	void SetTargetDoor(UDoorComp* NewTargetDoor);

	UFUNCTION(BlueprintCallable, Category = "Key")
	bool UseKey();
};
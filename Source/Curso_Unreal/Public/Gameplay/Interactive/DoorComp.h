// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoorComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockUnlocked, UDoorComp*, DoorComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UDoorComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorComp();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
	uint8 RequiredKeys = 0;

public:	
	
	UPROPERTY(BlueprintAssignable, Category="Door")
	FOnLockUnlocked OnLockUnlocked;
	
	UFUNCTION(BlueprintCallable, Category="Door")
	void AddLock(uint8 Keys = 1);

	UFUNCTION(BlueprintCallable, Category="Door")
	void RemoveLock(uint8 Keys = 1);

	UFUNCTION(BlueprintCallable, Category="Door")
	bool IsLocked() const;


		
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CURSO_UNREAL_API UkeyComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UkeyComp();

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
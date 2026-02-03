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
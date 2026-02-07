// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComp.generated.h"

// Forward declaration
class UInventoryItemObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSO_UNREAL_API UInventoryComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	TArray< TObjectPtr< UInventoryItemObject > > Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	int Size = 1;

	UFUNCTION()
	bool AddItemOnEmptySlot(UInventoryItemDefinition* ItemDefinition, int Quantity);

public:	
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray< UInventoryItemObject* >& GetItems() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int GetItemQuantity(UInventoryItemDefinition* ItemDefinition) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	UInventoryItemObject* GetItemAt(int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UInventoryItemDefinition* ItemDefinition, int Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UInventoryItemDefinition* ItemDefinition, int Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemAt(int Index, int Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortItems();		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Inventory/InventoryComp.h"
#include "Gameplay/Inventory/InventoryItem.h"

// Sets default values for this component's properties
UInventoryComp::UInventoryComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();

	ensure(Items.Num() <= Size && "Inventory size exceeded in Component, inventory will be truncated");

	if (Items.Num() > Size)
	{
		Items.SetNum(Size);

	}
	
	for(TObjectPtr<UInventoryItemObject> Item : Items)
	{
		if (Item)
		{
			ensure(Item->StackAmount <= Item->ItemDefinition->MaxStackAmount && "Item Inventory max stack exceeded in Component, inventory will be truncated");
			if (!Item || Item->StackAmount > Item->ItemDefinition->MaxStackAmount)
			{
				Item->StackAmount = Item->ItemDefinition->MaxStackAmount;
			}
		}
	}

	//Add empty slots
	for(int i = Items.Num(); i < Size; i++)
	{
		Items.Add(nullptr);
	}
}

bool UInventoryComp::AddItemOnEmptySlot(UInventoryItemDefinition* ItemDefinition, int Quantity)
{
	int RemainingQuantity = Quantity;
	for(int i = 0; i < Items.Num(); i++)
	{
		UInventoryItemObject* Item = Items[i];
		if (!Item)
		{
			Item = NewObject<UInventoryItemObject>(this);
			Item->ItemDefinition = ItemDefinition;
			if (RemainingQuantity > ItemDefinition->MaxStackAmount)
			{
				Item->StackAmount = ItemDefinition->MaxStackAmount;
				RemainingQuantity -= ItemDefinition->MaxStackAmount;
				Items[i] = Item;
			}
			else
			{
				Item->StackAmount = RemainingQuantity;
				Items[i] = Item;
				return true;
			}		
		}
	}

	while (RemainingQuantity > 0 && Items.Num() < Size)
	{
		UInventoryItemObject* Item = NewObject<UInventoryItemObject>(this);
		Item->ItemDefinition = ItemDefinition;
		if (RemainingQuantity > ItemDefinition->MaxStackAmount)
		{
			Item->StackAmount = ItemDefinition->MaxStackAmount;
			RemainingQuantity -= ItemDefinition->MaxStackAmount;
			Items.Add(Item);
		}
		else
		{
			Item->StackAmount = RemainingQuantity;
			Items.Add(Item);
			return true;
		}
	}
	return false;
}

const TArray< UInventoryItemObject* >& UInventoryComp::GetItems() const
{
	return Items;
}

int UInventoryComp::GetItemQuantity(UInventoryItemDefinition* ItemDefinition) const
{
	int Quantity = 0;
	for(TObjectPtr<UInventoryItemObject> Item : Items)
	{
		if (Item && Item->ItemDefinition == ItemDefinition)
		{
			Quantity += Item->StackAmount;
		}
	}
	return Quantity;
}

UInventoryItemObject* UInventoryComp::GetItemAt(int Index) const
{
	return Items[Index];
}

bool UInventoryComp::AddItem(UInventoryItemDefinition* ItemDefinition, int Quantity)
{
	check(ItemDefinition);
	check(Quantity > 0);
	int RemainingQuantity = Quantity;
	//Check if the item is already in the inventory
	for(TObjectPtr<UInventoryItemObject> Item : Items)
	{
		if (Item && Item->ItemDefinition == ItemDefinition && Item->StackAmount < Item->ItemDefinition->MaxStackAmount)
		{
			//Check if the item can be stacked
			if (Item->StackAmount + RemainingQuantity > Item->ItemDefinition->MaxStackAmount)
			{
				if (Item->StackAmount < Item->ItemDefinition->MaxStackAmount)
				{
					RemainingQuantity -= Item->ItemDefinition->MaxStackAmount - Item->StackAmount;
					Item->StackAmount = Item->ItemDefinition->MaxStackAmount;
				}
			}
			else
			{
				Item->StackAmount += RemainingQuantity;
				return true;
			}
		}
	}

	//If the item is not in the inventory
	return AddItemOnEmptySlot(ItemDefinition, RemainingQuantity);
}

bool UInventoryComp::RemoveItem(UInventoryItemDefinition* ItemDefinition, int Quantity)
{
	int RemainingQuantity = Quantity;
	bool bRemoved = false;
	for (int i = 0; i < Items.Num(); i++)
	{
		UInventoryItemObject* Item = Items[i];
		if (Item && Item->ItemDefinition == ItemDefinition)
		{
			if (Item->StackAmount < RemainingQuantity)
			{
				RemainingQuantity -= Item->StackAmount;
				Items[i] = nullptr;
			}
			else
			{
				Item->StackAmount -= RemainingQuantity;
				bRemoved = true;
				if (Item->StackAmount == 0) {
					Items[i] = nullptr;
				}
				break;
			}
		}
	}

	//Add empty slots
	for(int i = Items.Num(); i < Size; i++)
	{
		Items.Add(nullptr);
	}

	return bRemoved;
}

bool UInventoryComp::RemoveItemAt(int Index, int Quantity)
{
	if (Index < Items.Num() && Items[Index])
	{
		if(Items[Index]->StackAmount < Quantity)
		{
			return false;
		}
		else
		{
			Items[Index]->StackAmount -= Quantity;
			if (Items[Index]->StackAmount == 0) {
				Items[Index] = nullptr;
			}
			return true;
		}
	}
	return false;
}

bool LesserItem(const TObjectPtr<UInventoryItemObject>& A, const  TObjectPtr <UInventoryItemObject>& B)
{
	return (A && !B) || (A && B && (A->ItemDefinition->ItemName.CompareTo(B->ItemDefinition->ItemName) < 0));
}

void UInventoryComp::SortItems()
{
	Items.Sort(&LesserItem);
}

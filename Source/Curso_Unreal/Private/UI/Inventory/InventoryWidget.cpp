// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Gameplay/Inventory/InventoryItem.h"
#include "Gameplay/Inventory/InventoryComp.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UInventoryItemWidget::SetItemData(UInventoryItemObject* ItemObject)
{
    UTexture2D* Texture = EmptyItemTexture.Get(); // explícito
    FText ItemAmount = FText::GetEmpty();

    if (!ItemImage || !QuantityItemText)
    {
    
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ItemImage or QuantityItemText not found"); // debu
        ItemAmount = FText::FromString(*GetClass()->GetName());
    }

    if (IsValid(ItemObject) && IsValid(ItemObject->ItemDefinition))
    {
        if (IsValid(ItemObject->ItemDefinition->ItemIcon))
        {
            Texture = ItemObject->ItemDefinition->ItemIcon;
        }

        if (ItemObject->StackAmount > 1)
        {
            ItemAmount = FText::AsNumber(ItemObject->StackAmount);
        }

        if (NameItemText && bShowName)
        {
            NameItemText->SetText(ItemObject->ItemDefinition->ItemName);
        }
    }

    if (!IsValid(Texture))
    {
        UE_LOG(LogTemp, Error, TEXT("Texture INVALID. Empty=%s Icon=%s"),
            *GetNameSafe(EmptyItemTexture.Get()),
            ItemObject && ItemObject->ItemDefinition ? *GetNameSafe(ItemObject->ItemDefinition->ItemIcon) : TEXT("None"));
        return;
    }

    ItemImage->SetBrushFromTexture(Texture, false);
    QuantityItemText->SetText(ItemAmount);

    OnSetItemData(ItemObject);
}


void UInventoryGridWidget::SetInventoryItems(UInventoryComp* InventoryItems)
{
	check(InventoryItems);

    InventoryReference = InventoryItems;

	GridPanel->ClearChildren();
	const TArray< UInventoryItemObject* >& Items = InventoryItems->GetItems();
	
	for(int i = 0; i < Items.Num(); i++)
	{
		UInventoryItemWidget* InventoryItemWidget = CreateWidget<UInventoryItemWidget>(GetWorld(), InventoryItemWidgetClass);
		InventoryItemWidget->SetItemData(Items[i]);

		UUniformGridSlot* ActualSlot = GridPanel->AddChildToUniformGrid(InventoryItemWidget);
		ActualSlot->SetColumn(i % Columns);
		ActualSlot->SetRow(i / Columns);
	}
}
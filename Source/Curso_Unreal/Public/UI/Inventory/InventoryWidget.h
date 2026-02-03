// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryItemObject;
class UImage;
class UTextBlock;


/**
 * 
 */
UCLASS()
class CURSO_UNREAL_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityItemText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> NameItemText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowName = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> EmptyItemTexture;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemData(UInventoryItemObject *ItemObject);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnSetItemData(UInventoryItemObject* ItemObject);
};

class UUniformGridPanel;
class UInventoryComp;

UCLASS()
class CURSO_UNREAL_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 Columns = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass = nullptr;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetInventoryItems(UInventoryComp* InventoryItems);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI | Items")
	TWeakObjectPtr<UInventoryComp> InventoryReference;
};

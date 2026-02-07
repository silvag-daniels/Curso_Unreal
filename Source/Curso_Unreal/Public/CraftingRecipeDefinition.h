#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CraftingRecipeDefinition.generated.h"

class UInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FCraftingIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryItemDefinition> Item = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity = 1;
};

UCLASS(BlueprintType)
class CURSO_UNREAL_API UCraftingRecipeDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	TArray<FCraftingIngredient> Ingredients;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	FCraftingIngredient Result = nullptr;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotifySendGameplayEvent.generated.h"

/**
 * 
 */

UCLASS(editinlinenew, Const, hideCategories = Object, collapseCategories, Meta = (DisplayName = "GameplayEvent (Burst)"))
class CURSO_UNREAL_API UAnimNotifySendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotifySendGameplayEvent();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif // #if WITH_EDITOR

protected:

	// GameplayCue tag to invoke.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEvent, meta = (Categories = "GameplayEvent"))
	FGameplayTag GameplayEventTag;
	
};

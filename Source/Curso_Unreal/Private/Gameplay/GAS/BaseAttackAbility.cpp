// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/BaseAttackAbility.h"
#include "Gameplay/Combat/TargetingComp.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Framework/InGamePlayerController.h"

UBaseAttackAbility::UBaseAttackAbility()
{
	//Add tag
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Attack.Base")));
}

void UBaseAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	AInGamePlayerController* PlayerController = Cast<AInGamePlayerController>(Character->GetController());

	UTargetingComp* Target = PlayerController->GetTarget();
	UAbilitySystemComponent* TargetASC = Target->GetOwner()->GetComponentByClass<UAbilitySystemComponent>();
	

	if (TargetASC)
	{
		UAbilitySystemComponent* ASC = Character->GetComponentByClass<UAbilitySystemComponent>();
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(Character);

		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(ASC, PlayerController->AttackDamageEffect);

		ASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC, 1.0f, ContextHandle);
		FGameplayCueParameters CueParams;
		CueParams.Instigator = Character;
		CueParams.Location = Character->GetActorLocation();
		CueParams.TargetAttachComponent = Character->GetRootComponent();

		TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Combat.Attack"), CueParams);
	}

	
}

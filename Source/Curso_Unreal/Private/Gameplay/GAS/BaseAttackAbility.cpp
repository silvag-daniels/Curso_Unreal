// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/BaseAttackAbility.h"
#include "Gameplay/Combat/TargetingComp.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Framework/InGamePlayerController.h"
#include "Gameplay/Combat/CombatComp.h"
#include "Gameplay/GAS/CharacterAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UBaseAttackAbility::UBaseAttackAbility()
{
	//Add tag
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Attack.Base")));
}

void UBaseAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	AInGamePlayerController* PlayerController = Cast<AInGamePlayerController>(Character->GetController());

	Target = PlayerController->GetTarget();
	UAbilitySystemComponent* TargetASC = Target->GetOwner()->GetComponentByClass<UAbilitySystemComponent>();

	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Attack.Base.AnimationFinished"));
	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag, nullptr, true);

	if(WaitGameplayEventTask)
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UBaseAttackAbility::OnGameplayEventReceived);
		WaitGameplayEventTask->ReadyForActivation();
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	if (TargetASC)
	{
		FGameplayCueParameters CueParams;
		CueParams.Instigator = Character;
		CueParams.Location = Character->GetActorLocation();
		CueParams.TargetAttachComponent = Character->GetRootComponent();

		TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Combat.Attack"), CueParams);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBaseAttackAbility::OnGameplayEventReceived(const FGameplayEventData Payload)
{
	if(!Target.IsValid())
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetCurrentActorInfo()->AvatarActor.Get());
	AInGamePlayerController* PlayerController = Cast<AInGamePlayerController>(Character->GetController());

	UAbilitySystemComponent* TargetASC = Target->GetOwner()->GetComponentByClass<UAbilitySystemComponent>();

	if (TargetASC)
	{
		UAbilitySystemComponent* ASC = Character->GetComponentByClass<UAbilitySystemComponent>();
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(Character);

		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(ASC, PlayerController->AttackDamageEffect);

		ASC->ApplyGameplayEffectToTarget(GameplayEffect, TargetASC, 1.0f, ContextHandle);

	}
}

bool UBaseAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;

	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return false;
	}

	UCombatComp* CombatComp = Character->GetComponentByClass<UCombatComp>();
	if (!CombatComp || !CombatComp->GetTarget())
	{
		return false;
	}

	float AttackRange = 0.0f;
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		AttackRange = ASC->GetNumericAttribute(UCharacterAttributeSet::GetAttackRangeAttribute());
	}

	const float DistanceToTarget = FVector::Dist(Character->GetActorLocation(), CombatComp->GetTarget()->GetOwner()->GetActorLocation());
	if (DistanceToTarget > AttackRange)
	{
		return false;
	}

	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/BaseMovementAbility.h"
#include "Gameplay/Combat/TargetingComp.h"
#include "Gameplay/Combat/CombatComp.h"
#include "Gameplay/Framework/InGamePlayerController.h"
#include "Gameplay/GAS/CharacterAttributeSet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UBaseMovementAbility::UBaseMovementAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Movement.Base")));

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Movement.Base"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UBaseMovementAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UTargetingComp* Target = Cast<UTargetingComp>(TriggerEventData->OptionalObject);

	FGameplayTag NotifyTag = FGameplayTag::RequestGameplayTag("Abilities.Movement.NotifyInputFinished");
	UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, NotifyTag);

	if(WaitTask)
	{
		WaitTask->EventReceived.AddDynamic(this, &UBaseMovementAbility::OnEventNotifyInputReleased);
		WaitTask->ReadyForActivation();
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	if(!Character)
	{
		FinishAbility();
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

	if(!PlayerController)
	{
		FinishAbility();
		return;
	}

	BaseMovementTask = UBaseMovementAbilityTask::CreateBaseMovementTask(this);
	BaseMovementTask->OnAbilityFinished.AddDynamic(this, &UBaseMovementAbility::FinishAbility);

}

void UBaseMovementAbility::OnEventNotifyInputReleased(FGameplayEventData Payload)
{
	FGameplayTag NotifyTag = FGameplayTag::RequestGameplayTag("Abilities.Movement.NotifyInputFinished");

	if (Payload.EventTag == NotifyTag)
	{
		NotiifyInputReleased();
	}
}

void UBaseMovementAbility::NotiifyInputReleased()
{
	if (BaseMovementTask)
	{
		BaseMovementTask->NotifyInputReleased();
	}
}

void UBaseMovementAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (BaseMovementTask)
	{
		BaseMovementTask->OnAbilityFinished.RemoveDynamic(this, &UBaseMovementAbility::FinishAbility);
		BaseMovementTask->EndTask();
		BaseMovementTask = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseMovementAbility::FinishAbility()
{
	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}

UBaseMovementAbilityTask::UBaseMovementAbilityTask()
{
	bTickingTask = true;
}

void UBaseMovementAbilityTask::Activate()
{
	Super::Activate();

	if (Ability && Ability->GetCurrentActorInfo())
	{

		MyCharacter = Cast<ACharacter>(Ability->GetCurrentActorInfo()->AvatarActor.Get());

		if(MyCharacter.IsValid() && MyCharacter->GetCharacterMovement())
		{
			MyCharacter->GetCharacterMovement()->StopMovementImmediately();
			MyPlayerController = Cast<APlayerController>(MyCharacter->GetController());
		}
	}
	else
	{
		EndTask();
	}
}

UBaseMovementAbilityTask* UBaseMovementAbilityTask::CreateBaseMovementTask(UGameplayAbility* OwningAbility, float TresholdTime)
{
	UBaseMovementAbilityTask* Task = NewAbilityTask<UBaseMovementAbilityTask>(OwningAbility);
	Task->TresholdTime = TresholdTime;
	Task->ReadyForActivation();

	return Task;
}

void UBaseMovementAbilityTask::NotifyInputReleased()
{
	if (FollowTime < TresholdTime && !bIsPerformingSMTL)
	{
		UCombatComp* CombatComp = MyCharacter->GetComponentByClass<UCombatComp>();
		Target = CombatComp->GetTarget();

		if (Target.IsValid())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(MyPlayerController.Get(), Target->GetOwner());
			bIsPerformingChase = true;
		}
		else
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(MyPlayerController.Get(), Destination);
		}
		bIsPerformingSMTL = true;
	}
	else
	{
		MyCharacter->GetCharacterMovement()->StopMovementImmediately();
		OnAbilityFinished.Broadcast();
		EndTask();
	}
}

void UBaseMovementAbilityTask::TickTask(float DeltaTime)
{
	if (bIsPerformingSMTL)
	{
		bool bIsMoving = MyCharacter->GetCharacterMovement()->Velocity.SizeSquared() > UCharacterMovementComponent::BRAKE_TO_STOP_VELOCITY;
		bool bIsCloseToTarget = false;

		if(bIsPerformingChase)
		{
			UAbilitySystemComponent* ASC = Ability->GetAbilitySystemComponentFromActorInfo();

			if (ASC)
			{
				float attackRange = ASC->GetNumericAttribute(UCharacterAttributeSet::GetAttackRangeAttribute());
				bIsCloseToTarget = FVector::Dist(Target->GetOwner()->GetActorLocation(), MyCharacter->GetActorLocation()) < attackRange;
			}
		}

		if (bIsCloseToTarget)
		{
			FGameplayTag AbilityTag = Ability->AbilityTags.Last();
			UCombatComp* CombatComp = MyCharacter->FindComponentByClass<UCombatComp>();
			CombatComp->SetTarget(Target.Get());
			Cast<AInGamePlayerController>(MyPlayerController)->NotifyDestinationReached(AbilityTag, false);
		}

		if (!bIsMoving || bIsCloseToTarget)
		{
			NotifyInputReleased();
		}
	}
	else
	{
		FollowTime += DeltaTime;

		FHitResult HitResult;

		if (MyPlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
		{
			Destination = HitResult.Location;
		}

		FVector WorldDirection = (Destination - MyCharacter->GetActorLocation()).GetSafeNormal();

		MyCharacter->AddMovementInput(WorldDirection);

	}
	
}

void UBaseMovementAbilityTask::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

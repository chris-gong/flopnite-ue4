// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/SprintAbility.h"
#include "FNCharacter.h"
#include "FNCharacterMovementComponent.h"

USprintAbility::USprintAbility() {
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	const FGameplayTag& SprintAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Sprint"));
	AbilityTags.AddTag(SprintAbilityTag);
	ActivationOwnedTags.AddTag(SprintAbilityTag);
	AbilityInputID = EFNAbilityInputID::Sprint;
}

void USprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	//UE_LOG(LogTemp, Warning, TEXT("ability activated"));
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}

		if (AFNCharacter* Character = CastChecked<AFNCharacter>(ActorInfo->AvatarActor.Get())) {
			
			UFNCharacterMovementComponent* CharacterMovementComponent = CastChecked<UFNCharacterMovementComponent>(Character->GetMovementComponent());
			if (CharacterMovementComponent) {
				if (!SprintEffect)
				{
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
					return;
				}
				// set sprinting gameplay tags
				SprintEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, NewObject<UGameplayEffect>(this, SprintEffect), 1);
				// set a sprinting flag in character movement component to true
				CharacterMovementComponent->RequestToStartSprinting = true;
				//UE_LOG(LogTemp, Warning, TEXT("sprint ability successfully activated"));

				return;
			}
		}

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

bool USprintAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		if (ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get())) {
			UFNCharacterMovementComponent* CharacterMovementComponent = CastChecked<UFNCharacterMovementComponent>(Character->GetMovementComponent());
			if (CharacterMovementComponent && !CharacterMovementComponent->IsFalling()) {
				return true;
			}
		}
	}
	return false;
}

void USprintAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) {
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void USprintAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) {
	// set a sprinting flag in character movement component to false
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (AFNCharacter* Character = CastChecked<AFNCharacter>(ActorInfo->AvatarActor.Get())) {

		if (UFNCharacterMovementComponent* CharacterMovementComponent = CastChecked<UFNCharacterMovementComponent>(Character->GetMovementComponent())) {
			// set sprinting gameplay tags
			if (SprintEffectHandle.IsValid()) {
				BP_RemoveGameplayEffectFromOwnerWithHandle(SprintEffectHandle);
			}
			// set a sprinting flag in character movement component to true
			CharacterMovementComponent->RequestToStartSprinting = false;
			//UE_LOG(LogTemp, Warning, TEXT("sprint ability successfully cancelled"));

		}
	}
}
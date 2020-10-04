// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/SprintAbility.h"
#include "FNCharacter.h"
#include "FNCharacterMovementComponent.h"

USprintAbility::USprintAbility() {
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
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

		AFNCharacter* Character = CastChecked<AFNCharacter>(ActorInfo->AvatarActor.Get());

		if (Character) {
			
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
				UE_LOG(LogTemp, Warning, TEXT("sprint ability successfully activated"));

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
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Character) {
			// TODO: check if player is on the ground or not, or if they're aiming down sight, and etc.
			return true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Cannot activate ability 1"));
			return false;
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Cannot activate ability 2"));
		return false;
	}
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

	AFNCharacter* Character = CastChecked<AFNCharacter>(ActorInfo->AvatarActor.Get());

	if (Character) {

		UFNCharacterMovementComponent* CharacterMovementComponent = CastChecked<UFNCharacterMovementComponent>(Character->GetMovementComponent());
		if (CharacterMovementComponent) {
			// set sprinting gameplay tags
			if (SprintEffectHandle.IsValid()) {
				BP_RemoveGameplayEffectFromOwnerWithHandle(SprintEffectHandle);
			}
			// set a sprinting flag in character movement component to true
			CharacterMovementComponent->RequestToStartSprinting = false;
			UE_LOG(LogTemp, Warning, TEXT("sprint ability successfully cancelled"));

		}
	}
}
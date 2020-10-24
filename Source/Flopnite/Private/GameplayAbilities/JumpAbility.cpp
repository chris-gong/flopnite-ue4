


#include "GameplayAbilities/JumpAbility.h"
#include "FNAbilitySystemComponent.h"
#include "FNCharacter.h"
#include "FNCharacterMovementComponent.h"

UJumpAbility::UJumpAbility() {
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	const FGameplayTag& JumpAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	AbilityTags.AddTag(JumpAbilityTag);
	ActivationOwnedTags.AddTag(JumpAbilityTag);
	AbilityInputID = EFNAbilityInputID::Jump;
}

void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
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
				if (!JumpEffect)
				{
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
					return;
				}
				// set jumping gameplay tags
				//JumpEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, NewObject<UGameplayEffect>(this, JumpEffect), 1);
				// jump
				Character->Jump();

				return;
			}
		}

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

bool UJumpAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Character && Character->CanJump()) {
			return true;
		}
	}
	return false;
}

void UJumpAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) {
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UJumpAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) {
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("jump ability cancelled"));
		if (JumpEffectHandle.IsValid()) {
			// there's a race condition where cancel ability, when called by gameplay tag event function, the jump effect handle is not valid
			//UE_LOG(LogTemp, Warning, TEXT("jump effect handle is valid"));
			BP_RemoveGameplayEffectFromOwnerWithHandle(JumpEffectHandle);
		}
		
		Character->StopJumping();
	}
}


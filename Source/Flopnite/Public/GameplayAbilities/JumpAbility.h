

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/FNGameplayAbility.h"
#include "JumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UJumpAbility : public UFNGameplayAbility
{
	GENERATED_BODY()
	
public:
	UJumpAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TSubclassOf<UGameplayEffect> JumpEffect;

	FActiveGameplayEffectHandle JumpEffectHandle;
};

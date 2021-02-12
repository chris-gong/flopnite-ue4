// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "FNAT_PlayMontageAndWaitForEvent.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UFNAT_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static UFNAT_PlayMontageAndWaitForEvent* CreatePlayMontageAndWaitForEventProxy(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		FGameplayTagContainer EventTags,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.f, 
		float StartTimeSeconds = 0.f);

	virtual void Activate() override;

private:
	/** List of tags to match against gameplay events */
	UPROPERTY()
	FGameplayTagContainer EventTags;
};

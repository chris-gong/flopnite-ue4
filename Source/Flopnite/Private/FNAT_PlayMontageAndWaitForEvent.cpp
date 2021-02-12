// Fill out your copyright notice in the Description page of Project Settings.


#include "FNAT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemGlobals.h"

UFNAT_PlayMontageAndWaitForEvent* UFNAT_PlayMontageAndWaitForEvent::CreatePlayMontageAndWaitForEventProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	UAnimMontage* AnimMontageToPlay, FGameplayTagContainer EventTags, float PlayRate, FName AnimStartSection, bool bStopWhenGameplayAbilityEnds, float RootMotionTranslationScale, float AnimStartTimeSeconds)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(PlayRate);

	UFNAT_PlayMontageAndWaitForEvent* UFNAT_PlayMontageAndWaitForEventObj = NewAbilityTask<UFNAT_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	UFNAT_PlayMontageAndWaitForEventObj->EventTags = EventTags;

	return UFNAT_PlayMontageAndWaitForEventObj;
}

void UFNAT_PlayMontageAndWaitForEvent::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("AbilityTask actiavated"));
}
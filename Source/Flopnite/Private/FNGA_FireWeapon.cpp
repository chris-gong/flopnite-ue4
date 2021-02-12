// Fill out your copyright notice in the Description page of Project Settings.


#include "FNGA_FireWeapon.h"

UFNGA_FireWeapon::UFNGA_FireWeapon()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.ChangingWeapon")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Fire")));
}

void UFNGA_FireWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (AbilityID == EFNAbilityInputID::FirePistol)
	{
		UE_LOG(LogTemp, Warning, TEXT("fire pistol ability activated"));

	}
	else if (AbilityID == EFNAbilityInputID::FireRifle)
	{
		UE_LOG(LogTemp, Warning, TEXT("fire rifle ability activated"));
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

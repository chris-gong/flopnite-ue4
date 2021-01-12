// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeaponAbility.h"

UFireWeaponAbility::UFireWeaponAbility() {
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.ChangingWeapon")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Fire")));
}
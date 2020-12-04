// Fill out your copyright notice in the Description page of Project Settings.


#include "FNGameplayAbility.h"

UFNGameplayAbility::UFNGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
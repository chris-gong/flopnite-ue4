// Fill out your copyright notice in the Description page of Project Settings.


#include "FNAbilitySystemComponent.h"
#include "GeneratedCodeHelpers.h"

UFNAbilitySystemComponent::UFNAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
	CharacterAbilitiesGiven = false;
}

void UFNAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFNAbilitySystemComponent, CharacterAbilitiesGiven);
}
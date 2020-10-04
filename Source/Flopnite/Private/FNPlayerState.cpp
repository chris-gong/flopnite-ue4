// Fill out your copyright notice in the Description page of Project Settings.


#include "FNPlayerState.h"
#include "FNAbilitySystemComponent.h"
#include "FNAttributeSet.h"

AFNPlayerState::AFNPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UFNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UFNAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 10.f;
}

UAbilitySystemComponent* AFNPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

UFNAttributeSet* AFNPlayerState::GetAttributeSet() const {
	return AttributeSet;
}
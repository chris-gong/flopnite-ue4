// Fill out your copyright notice in the Description page of Project Settings.


#include "FNPlayerState.h"
#include "FNAbilitySystemComponent.h"

AFNPlayerState::AFNPlayerState()
{
    // Create ability system component, and set it to be explicitly replicated
    AbilitySystemComponent = CreateDefaultSubobject<UFNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    // Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another FNPlayerState receives a GE,
    // we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create the attribute set, this replicates by default
    // Adding it as a subobject of the owning actor of an AbilitySystemComponent
    // automatically registers the AttributeSet with the AbilitySystemComponent
    //AttributeSetBase = CreateDefaultSubobject<UGDAttributeSetBase>(TEXT("AttributeSetBase"));

    // Set PlayerState's NetUpdateFrequency to the same as the Character.
    // Default is very low for PlayerStates and introduces perceived lag in the ability system.
    // 100 is probably way too high for a shipping game, you can adjust to fit your needs.
    NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* AFNPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
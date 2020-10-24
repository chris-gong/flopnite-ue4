// Fill out your copyright notice in the Description page of Project Settings.


#include "FNPlayerState.h"
#include "FNCharacter.h"
#include "FNAbilitySystemComponent.h"
#include "FNAttributeSet.h"

AFNPlayerState::AFNPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UFNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UFNAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 10.f;
}

void AFNPlayerState::BeginPlay()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AFNPlayerState::JumpTagChanged);
}

UAbilitySystemComponent* AFNPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

UFNAttributeSet* AFNPlayerState::GetAttributeSet() const {
	return AttributeSet;
}

void AFNPlayerState::JumpTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// new count is greater than zero when the tag is being added
	if (NewCount > 0)
	{
		if (AFNCharacter* OwningChar = CastChecked<AFNCharacter>(GetPawn()))
		{
			FGameplayTagContainer AbilityTagsToCancel;
			AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));

			FGameplayTagContainer AbilityTagsToIgnore;
			UE_LOG(LogTemp, Warning, TEXT("about to cancel abilities"));

			AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
			UE_LOG(LogTemp, Warning, TEXT("jump enabled set to true"));
			OwningChar->JumpEnabled = true;
		}
	}
}

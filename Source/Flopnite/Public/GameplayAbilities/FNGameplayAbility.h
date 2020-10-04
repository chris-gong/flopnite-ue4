// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FNAbilitySystemComponent.h"
#include "FNGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UFNGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY()
	EFNAbilityInputID AbilityInputID;
};

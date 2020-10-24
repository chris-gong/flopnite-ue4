// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FNAbilitySystemComponent.generated.h"

/**
 * 
 */

// abilities
UENUM(BlueprintType)
enum class EFNAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
	Sprint			UMETA(DisplayName = "Sprint"), // shift
	Jump			UMETA(DisplayName = "Jump") // space
};

UCLASS()
class FLOPNITE_API UFNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool CharacterAbilitiesGiven = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "FNPlayerState.generated.h"

/**
 * 
 */
class UFNAbilitySystemComponent;

UCLASS()
class FLOPNITE_API AFNPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFNPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	UFNAbilitySystemComponent* AbilitySystemComponent;
};

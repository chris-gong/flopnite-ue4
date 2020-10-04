// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "FNPlayerState.generated.h"

class UFNAbilitySystemComponent;
class UFNAttributeSet;
/**
 * 
 */
UCLASS()
class FLOPNITE_API AFNPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AFNPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UFNAttributeSet* GetAttributeSet() const;

protected:
	// ability system component on the owning actor
	UPROPERTY()
	UFNAbilitySystemComponent* AbilitySystemComponent;

	// attribute set
	UPROPERTY()
	UFNAttributeSet* AttributeSet;
};

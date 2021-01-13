// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FNGameplayAbility.h"
#include "FireWeaponAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UFireWeaponAbility : public UFNGameplayAbility
{
	GENERATED_BODY()
public:
	UFireWeaponAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon")
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	float Damage;
};

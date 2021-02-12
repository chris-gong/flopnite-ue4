// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FNGameplayAbility.h"
#include "FNGA_FireWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UFNGA_FireWeapon : public UFNGameplayAbility
{
	GENERATED_BODY()
public:
	UFNGA_FireWeapon();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	UAnimMontage* FireMontage;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon")
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	float Damage;
};

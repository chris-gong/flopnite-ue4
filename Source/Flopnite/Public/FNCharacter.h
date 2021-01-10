// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "FNCharacter.generated.h"

class UFNAbilitySystemComponent;
class UFNGameplayAbility;
class UGameplayEffect;

UCLASS(config=Game)
class AFNCharacter : public AALSCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFNCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void OnRep_PlayerState() override;

	UPROPERTY()
	UFNAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	bool ASCInputBound;
	
	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TArray<TSubclassOf<UFNGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TMap<FString, TSubclassOf<UFNGameplayAbility>> WeaponAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TSubclassOf<UGameplayEffect> ChangingWeaponEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TMap<FString, TSubclassOf<UGameplayEffect>> EquipWeaponEffects;

	UPROPERTY(Replicated)
	FGameplayAbilitySpecHandle EquippedWeaponAbilityHandle;

	UPROPERTY(Replicated)
	FActiveGameplayEffectHandle EquippedWeaponEffectHandle;

	UPROPERTY(Replicated)
	FActiveGameplayEffectHandle WeaponChangedEffectHandle;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	UFUNCTION()
	void BindASCInput();

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	void AddCharacterAbilities();

	void InitializeAttributes();

	virtual void SetOverlayState(EALSOverlayState NewState) override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	virtual bool MantleCheck(const FALSMantleTraceSettings& TraceSettings,
		EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::ForOneFrame) override;

	UFUNCTION()
	void EquipWeapon();

	UFUNCTION()
	void UnEquipWeapon();

	
};


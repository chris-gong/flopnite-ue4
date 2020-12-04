// Copyright Epic Games, Inc. All Rights Reserved.

#include "FNCharacter.h"
#include "FNPlayerState.h"
#include "FNAbilitySystemComponent.h"
#include "Flopnite.h"
#include "FNGameplayAbility.h"

//////////////////////////////////////////////////////////////////////////
// AFNCharacter

AFNCharacter::AFNCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	ASCInputBound = false;
}

void AFNCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindASCInput();
}

UAbilitySystemComponent* AFNCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFNCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	AFNPlayerState* FNPlayerState = GetPlayerState<AFNPlayerState>();
	if (FNPlayerState)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UFNAbilitySystemComponent>(FNPlayerState->GetAbilitySystemComponent());

		FNPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FNPlayerState, this);

		AddCharacterAbilities();
	}
}

void AFNCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	AFNPlayerState* FNPlayerState = GetPlayerState<AFNPlayerState>();
    if (FNPlayerState)
    {
        // Set the ASC for clients. Server does this in PossessedBy.
        AbilitySystemComponent = Cast<UFNAbilitySystemComponent>(FNPlayerState->GetAbilitySystemComponent());

        // Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
        AbilitySystemComponent->InitAbilityActorInfo(FNPlayerState, this);

        // Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
        BindASCInput();
    }
}

void AFNCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent && InputComponent)
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
            FString("CancelTarget"), FString("EFNAbilityInputID"), static_cast<int32>(EFNAbilityInputID::Confirm), static_cast<int32>(EFNAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AFNCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UFNGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		// By default, make the level of each ability 1 until I find a purpose for making it a different value
		FGameplayAbilitySpec GameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this);
		AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

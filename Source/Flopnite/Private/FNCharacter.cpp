// Copyright Epic Games, Inc. All Rights Reserved.

#include "FNCharacter.h"
#include "FNPlayerState.h"
#include "FNAbilitySystemComponent.h"
#include "Flopnite.h"
#include "FNGameplayAbility.h"
#include "GeneratedCodeHelpers.h"

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

		InitializeAttributes();

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

		InitializeAttributes();
    }
}

void AFNCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFNCharacter, EquippedWeaponAbilityHandle);
	DOREPLIFETIME(AFNCharacter, EquippedWeaponEffectHandle);
	DOREPLIFETIME(AFNCharacter, WeaponChangedEffectHandle);
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

void AFNCharacter::InitializeAttributes() 
{
	if (AbilitySystemComponent) 
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NothingEquippedEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(*(EquipWeaponEffects.Find("Nothing")), 1, EffectContext);
		EquippedWeaponEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*(NothingEquippedEffectHandle.Data.Get()));
	}
}

void AFNCharacter::SetOverlayState(EALSOverlayState NewState)
{
	if (OverlayState != NewState)
	{
		UnEquipWeapon();
		const EALSOverlayState Prev = OverlayState;
		OverlayState = NewState;
		OnOverlayStateChanged(Prev);

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_SetOverlayState(NewState);
		}
	}
}

void AFNCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	EquipWeapon();
}

void AFNCharacter::EquipWeapon()
{
	if (AbilitySystemComponent)
	{
		// remove existing gameplay effect associated with changing weapons
		AbilitySystemComponent->RemoveActiveGameplayEffect(WeaponChangedEffectHandle);
		
		// add gameplay tag through gameplay effect indicating that the character has equipped a new weapon
		// as well as grant ability (through gameplay effect or calling give ability?)
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		
		if (OverlayState == EALSOverlayState::Rifle)
		{
			FGameplayEffectSpecHandle NothingEquippedEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(*(EquipWeaponEffects.Find("Rifle")), 1, EffectContext);
			EquippedWeaponEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*(NothingEquippedEffectHandle.Data.Get()));
		}
		else if (OverlayState == EALSOverlayState::PistolTwoHanded)
		{
			FGameplayEffectSpecHandle NothingEquippedEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(*(EquipWeaponEffects.Find("Pistol")), 1, EffectContext);
			EquippedWeaponEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*(NothingEquippedEffectHandle.Data.Get()));
		}
		else
		{
			FGameplayEffectSpecHandle NothingEquippedEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(*(EquipWeaponEffects.Find("Nothing")), 1, EffectContext);
			EquippedWeaponEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*(NothingEquippedEffectHandle.Data.Get()));
		}
	}
}

void AFNCharacter::UnEquipWeapon() 
{
	if (AbilitySystemComponent)
	{
		// remove existing gameplay effect associated with the previously equipped weapon
		AbilitySystemComponent->RemoveActiveGameplayEffect(EquippedWeaponEffectHandle);
		
		// add gameplay tag through gameplay effect indicating that the character is currently changing weapons
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle ChangingWeaponEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(ChangingWeaponEffect, 1, EffectContext);
		WeaponChangedEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*(ChangingWeaponEffectHandle.Data.Get()));

		// remove ability (through gameplay effect at the start or through clearability?)
	}
}
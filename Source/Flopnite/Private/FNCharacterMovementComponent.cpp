// Fill out your copyright notice in the Description page of Project Settings.


#include "FNCharacterMovementComponent.h"
#include "FNCharacter.h"
#include "GameFramework/Character.h"

UFNCharacterMovementComponent::UFNCharacterMovementComponent() {
	SprintSpeedMultiplier = 1.5f;
	RequestToStartSprinting = false;
}

float UFNCharacterMovementComponent::GetMaxSpeed() const {
	AFNCharacter* Owner = Cast<AFNCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (RequestToStartSprinting)
	{
		//UE_LOG(LogTemp, Warning, TEXT("request to start sprinting true"));

		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}
	//UE_LOG(LogTemp, Warning, TEXT("request to start sprinting false"));
	return Owner->GetMoveSpeed();
}

void UFNCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

// TODO: better understand what is going on in this function
FNetworkPredictionData_Client* UFNCharacterMovementComponent::GetPredictionData_Client() const {
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UFNCharacterMovementComponent* MutableThis = const_cast<UFNCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FFNNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UFNCharacterMovementComponent::FFNSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
}

uint8 UFNCharacterMovementComponent::FFNSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool UFNCharacterMovementComponent::FFNSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FFNSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}
	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UFNCharacterMovementComponent::FFNSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UFNCharacterMovementComponent* CharacterMovement = Cast<UFNCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
	}
}

void UFNCharacterMovementComponent::FFNSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);
}

UFNCharacterMovementComponent::FFNNetworkPredictionData_Client::FFNNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UFNCharacterMovementComponent::FFNNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FFNSavedMove());
}

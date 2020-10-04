// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FNCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UFNCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	class FFNSavedMove : public FSavedMove_Character
	{
	public:
		// typedef ... super is needed to override references to Super from UCharacterMovementComponent to this class' super class
		typedef FSavedMove_Character Super;

		///@brief Resets all saved variables.
		virtual void Clear() override;

		///@brief Store input commands in the compressed flags.
		virtual uint8 GetCompressedFlags() const override;

		///@brief This is used to check whether or not two moves can be combined into one.
		///Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		///@brief Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		///@brief Sets variables on character movement component before making a predictive correction.
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Sprint
		uint8 SavedRequestToStartSprinting : 1;
	};

	class FFNNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
	public:
		// typedef ... super is needed to override references to Super from UCharacterMovementComponent to this class' super class
		typedef FNetworkPredictionData_Client_Character Super;

		FFNNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

		///@brief Allocates a new copy of our custom saved move
		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	UFNCharacterMovementComponent();

	// how much speed will increase by when sprinting
	UPROPERTY()
	float SprintSpeedMultiplier;

	// property that indicates sprinting used for saved moves to be sent to the server
	uint8 RequestToStartSprinting : 1;

	// called in UE4 PerformMovement function when character moves
	virtual float GetMaxSpeed() const override;

	// used by the clients and server to decrypt and read flags from saved move
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	// override FNetworkPredicationData_Client class that is used to wrap saved move that is sent to the server
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
};

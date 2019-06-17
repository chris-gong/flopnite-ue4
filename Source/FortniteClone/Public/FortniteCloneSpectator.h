// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FortniteCloneSpectator.generated.h"


UCLASS()
class FORTNITECLONE_API AFortniteCloneSpectator : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFortniteCloneSpectator();

	float BaseTurnRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for up/down input */
	void MoveUp(float Value);

	void TurnAtRate(float Value);
};

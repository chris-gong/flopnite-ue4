// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "FNAbilitySystemComponent.h"
#include "FNCharacter.generated.h"

class UFlopniteAbilitySystemComponent;
class UFNAttributeSet;
class UGameplayEffect;
class UFNGameplayAbility;

UCLASS(config=Game)
class AFNCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AFNCharacter(const class FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// called on the server, this is where abilities added
	virtual void PossessedBy(AController* NewController) override;
	
	// called on the client, this is where ability system component is set because by this point asc will have been initialized
	virtual void OnRep_PlayerState() override;

	// retrieved from the player state since the character is the avator actor
	UPROPERTY()
	UFNAbilitySystemComponent* AbilitySystemComponent;

	// retrieved from the player state since the character is the avator actor
	UPROPERTY()
	UFNAttributeSet* AttributeSet;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability System")
	TArray<TSubclassOf<UFNGameplayAbility>> CharacterAbilities;

	UFUNCTION()
	void InitializeAttributes();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	float GetMoveSpeed() const;

	UPROPERTY()
	bool JumpEnabled;

private:
	UPROPERTY()
	bool ASCInputBound;

	// binds client input to ability system component abilities
	UFUNCTION()
	void BindASCInput();

	// on the server, adds abilities to the ability system component
	UFUNCTION()
	void AddCharacterAbilities();
};


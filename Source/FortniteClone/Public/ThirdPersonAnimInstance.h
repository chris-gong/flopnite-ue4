// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "ThirdPersonAnimInstance.generated.h"

class AFortniteCloneCharacter;
// This class does not need to be modified.
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UThirdPersonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UThirdPersonAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	bool HoldingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	bool AimedIn;

	/* Value for blend poses by int in blueprint */
	/* 0 for not holding anything, 1 for holding weapon on hip, 2 for holding weapon aimed into ironsights */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	int HoldingWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float WalkingX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float WalkingY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float RunningX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Generic")
	float RunningY;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};

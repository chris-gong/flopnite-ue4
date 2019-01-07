// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "ThirdPersonAnimInstance.generated.h"

// This class does not need to be modified.
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UThirdPersonAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool HoldingGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	float InterpSpeed;
};

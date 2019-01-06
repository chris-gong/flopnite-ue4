// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonAnimInstance.h"

// Add default functionality here for any IGuyAnimInstance functions that are not pure virtual.

UThirdPersonAnimInstance::UThirdPersonAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsWalking = false;
	IsRunning = false;
	HoldingGun = false;
};
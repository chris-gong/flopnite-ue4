// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonAnimInstance.h"

// Add default functionality here for any IGuyAnimInstance functions that are not pure virtual.

UThirdPersonAnimInstance::UThirdPersonAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsWalking = false;
	IsRunning = false;
	HoldingGun = false;
	Speed = 0.0;
	AimPitch = 0.0;
	AimYaw = 0.0;
	InterpSpeed = 15.0;
	TransformAlpha = 0.7;
	CameraRotation = FRotator(0, 0, 0);
};
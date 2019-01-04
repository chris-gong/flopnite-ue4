// Fill out your copyright notice in the Description page of Project Settings.

#include "GuyAnimInstance.h"

// Add default functionality here for any IGuyAnimInstance functions that are not pure virtual.

UGuyAnimInstance::UGuyAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsWalking = false;
	IsRunning = false;
	HoldingGun = false;
	ShotGun = false;
};
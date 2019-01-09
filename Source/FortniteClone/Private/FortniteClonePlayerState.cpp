// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerState.h"

AFortniteClonePlayerState::AFortniteClonePlayerState() {
	InBuildMode = false;
	BuildMode = FString("None");
	HoldingGun = false;
	AimedIn = false;
	EquippedWeapons.Add(0);
	CurrentWeapon = 0;
}
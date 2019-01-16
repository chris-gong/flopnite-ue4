// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerState.h"

AFortniteClonePlayerState::AFortniteClonePlayerState() {
	InBuildMode = false;
	BuildMode = FString("None");
	HoldingWeapon = false;
	HoldingBandage = false;
	AimedIn = false;
	EquippedWeapons.Add(0);
	CurrentWeapon = 0;
	JustShotShotgun = false;
	JustSwungPickaxe = false;
	JustUsedBandage = false;
}
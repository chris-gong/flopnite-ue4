// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerState.h"

AFortniteClonePlayerState::AFortniteClonePlayerState() {
	InBuildMode = false;
	BuildMode = FString("None");
	HoldingWeapon = false;
	HoldingBandage = false;
	AimedIn = false;
	EquippedWeapons.Add(0); //pickaxe
	EquippedWeaponsAmmunition.Add(0); // pickaxe
	EquippedWeaponsAmmunition.Add(0); // assault rifle
	EquippedWeaponsAmmunition.Add(0); // shotgun
	EquippedWeaponsClips.Add(0); // pickaxe
	EquippedWeaponsClips.Add(0); // assault rifle
	EquippedWeaponsClips.Add(0); // shotgun
	MaterialCounts.Add(0); // wood
	MaterialCounts.Add(0); // stone
	MaterialCounts.Add(0); // steel
	CurrentWeapon = 0;
	BandageCount = 0;
	JustShotShotgun = false;
	JustSwungPickaxe = false;
	JustUsedBandage = false;
	JustReloadedRifle = false;
	JustReloadedShotgun = false;
}
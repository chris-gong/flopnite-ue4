// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FortniteClonePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AFortniteClonePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AFortniteClonePlayerState();

	bool InBuildMode;
	FString BuildMode;
	bool HoldingWeapon;
	bool HoldingBandage;
	bool AimedIn;
	TArray<int> EquippedWeapons; //0 for pickaxe, 1 for assault rifle, 2 for shotgun
	TArray<int> EquippedWeaponsAmmunition; //index 1 holds ammo for assault rifle, index 2 holds ammo for shotgun
	//below will be used primarily for keeping track of how many bullets in a clip while switching between guns and building
	TArray<int> EquippedWeaponsClips; //index 1 holds current magazine clip count for assault rifle, index 2 holds current magazine clip count for shotgun
	TArray<int> MaterialCounts; //index 0 holds count for wood, index 1 holds count for stone, index 2 holds count for steel
	int CurrentWeapon; //0 for pickaxe, 1 for assault rifle, 2 for shotgun, -1 for non weapons like bandages
	int BandageCount;
	int KillCount;

	bool JustShotRifle; // used to prevent player from spamming rifle
	bool JustShotShotgun; // used to prevent player from spamming shotgun
	bool JustSwungPickaxe; //used to prevent player from spamming pickaxe
	bool JustUsedBandage; //used to prevent player from spamming bandage and doing other things while reloadingreloa
	bool JustReloadedRifle; //used to prevent player from doing other things while reloading
	bool JustReloadedShotgun; //used to prevent player from doing other things while reloading

protected:

private:

};

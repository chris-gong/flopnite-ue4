// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FortniteClonePlayerState.generated.h"

class AFortniteCloneCharacter;

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AFortniteClonePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AFortniteClonePlayerState();

	UPROPERTY(Replicated)
	bool InBuildMode;

	UPROPERTY(Replicated)
	FString BuildMode;

	UPROPERTY(Replicated)
	bool HoldingWeapon;

	UPROPERTY(Replicated)
	bool HoldingHealingItem;

	UPROPERTY(Replicated)
	bool AimedIn;

	UPROPERTY(Replicated)
	TArray<int> EquippedWeapons; //0 for pickaxe, 1 for assault rifle, 2 for shotgun

	UPROPERTY(Replicated)
	TArray<int> EquippedWeaponsAmmunition; //index 1 holds ammo for assault rifle, index 2 holds ammo for shotgun

	//below will be used primarily for keeping track of how many bullets in a clip while switching between guns and building
	UPROPERTY(Replicated)
	TArray<int> EquippedWeaponsClips; //index 1 holds current magazine clip count for assault rifle, index 2 holds current magazine clip count for shotgun

	UPROPERTY(Replicated)
	TArray<int> MaterialCounts; //index 0 holds count for wood, index 1 holds count for stone, index 2 holds count for steel

	UPROPERTY(Replicated)
	TArray<int> HealingItemCounts; //index 0 holds count for bandages, index 1 holds count for potions

	UPROPERTY(Replicated)
	int CurrentWeapon; //0 for pickaxe, 1 for assault rifle, 2 for shotgun, -1 for non weapons like bandages

	UPROPERTY(Replicated)
	int CurrentHealingItem; //0 for bandage, 1 for potion, -1 for non healing type items

	UPROPERTY(Replicated)
	int KillCount;

	UPROPERTY(Replicated)
	bool JustShotRifle; // used to prevent player from spamming rifle

	UPROPERTY(Replicated)
	bool JustShotShotgun; // used to prevent player from spamming shotgun

	UPROPERTY(Replicated)
	bool JustSwungPickaxe; //used to prevent player from spamming pickaxe

	UPROPERTY(Replicated)
	bool JustUsedHealingItem; //used to prevent player from spamming healing items and doing other things while reloading 

	UPROPERTY(Replicated)
	bool JustReloadedRifle; //used to prevent player from doing other things while reloading

	UPROPERTY(Replicated)
	bool JustReloadedShotgun; //used to prevent player from doing other things while reloading

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void Tick(float DeltaSeconds) override;
protected:

private:

};

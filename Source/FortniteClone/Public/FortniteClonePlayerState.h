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
	int CurrentWeapon; //0 for pickaxe, 1 for assault rifle, 2 for shotgun, -1 for non weapons like bandages
	int BandageCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool JustShotShotgun; // used to prevent player from spamming shotgun

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool JustSwungPickaxe; //used to prevent player from spamming pickaxe

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool JustUsedBandage; //used to prevent player from spamming bandage

protected:

private:

};

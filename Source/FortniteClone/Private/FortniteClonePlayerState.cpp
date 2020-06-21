// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerState.h"
#include "FortniteCloneCharacter.h"
#include "UnrealNetwork.h"

AFortniteClonePlayerState::AFortniteClonePlayerState() {
	InBuildMode = false;
	BuildMode = FString("None");
	HoldingWeapon = true; //when spawned, player is holding pickaxe
	HoldingHealingItem = false;
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
	CurrentHealingItem = -1;
	HealingItemCounts.Add(0);
	HealingItemCounts.Add(0);
	JustShotRifle = false;
	JustShotShotgun = false;
	JustSwungPickaxe = false;
	JustUsedHealingItem = false;
	JustReloadedRifle = false;
	JustReloadedShotgun = false;
	KillCount = 0;
}

void AFortniteClonePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFortniteClonePlayerState, InBuildMode);
	DOREPLIFETIME(AFortniteClonePlayerState, BuildMode);
	DOREPLIFETIME(AFortniteClonePlayerState, HoldingWeapon);
	DOREPLIFETIME(AFortniteClonePlayerState, HoldingHealingItem);
	DOREPLIFETIME(AFortniteClonePlayerState, EquippedWeapons);
	DOREPLIFETIME(AFortniteClonePlayerState, EquippedWeaponsAmmunition);
	DOREPLIFETIME(AFortniteClonePlayerState, EquippedWeaponsClips);
	DOREPLIFETIME(AFortniteClonePlayerState, MaterialCounts);
	DOREPLIFETIME(AFortniteClonePlayerState, CurrentWeapon);
	DOREPLIFETIME(AFortniteClonePlayerState, CurrentHealingItem);
	DOREPLIFETIME(AFortniteClonePlayerState, HealingItemCounts);
	DOREPLIFETIME(AFortniteClonePlayerState, JustShotRifle);
	DOREPLIFETIME(AFortniteClonePlayerState, JustShotShotgun);
	DOREPLIFETIME(AFortniteClonePlayerState, JustSwungPickaxe);
	DOREPLIFETIME(AFortniteClonePlayerState, JustUsedHealingItem);
	DOREPLIFETIME(AFortniteClonePlayerState, JustReloadedRifle);
	DOREPLIFETIME(AFortniteClonePlayerState, JustReloadedShotgun);
	DOREPLIFETIME(AFortniteClonePlayerState, KillCount);
	DOREPLIFETIME(AFortniteClonePlayerState, bIsSpectator);
}

void AFortniteClonePlayerState::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(GetPawn());
	if (FortniteCloneCharacter) {

	}
}
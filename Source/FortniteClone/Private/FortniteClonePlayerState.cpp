// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerState.h"
#include "UnrealNetwork.h"
#include "FortniteCloneCharacter.h"

AFortniteClonePlayerState::AFortniteClonePlayerState() {
	InBuildMode = false;
	BuildMode = FString("None");
	HoldingWeapon = true; //when spawned, player is holding pickaxe
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
	JustShotRifle = false;
	JustShotShotgun = false;
	JustSwungPickaxe = false;
	JustUsedBandage = false;
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
	DOREPLIFETIME(AFortniteClonePlayerState, HoldingBandage);
	DOREPLIFETIME(AFortniteClonePlayerState, EquippedWeapons);
	DOREPLIFETIME(AFortniteClonePlayerState, EquippedWeaponsAmmunition);
	DOREPLIFETIME(AFortniteClonePlayerState, EquippedWeaponsClips);
	DOREPLIFETIME(AFortniteClonePlayerState, MaterialCounts);
	DOREPLIFETIME(AFortniteClonePlayerState, CurrentWeapon);
	DOREPLIFETIME(AFortniteClonePlayerState, BandageCount);
	DOREPLIFETIME(AFortniteClonePlayerState, JustShotRifle);
	DOREPLIFETIME(AFortniteClonePlayerState, JustShotShotgun);
	DOREPLIFETIME(AFortniteClonePlayerState, JustSwungPickaxe);
	DOREPLIFETIME(AFortniteClonePlayerState, JustUsedBandage);
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
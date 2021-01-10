// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EFNAbilityInputID : uint8
{
	// 0 None
	None,
    // 1 Confirm Ability? TODO: do more research on confirming and cancelling abilities
    Confirm,
    // 2 Cancel Ability? 
    Cancel,
    // 4 Fire Rifle - Left Mouse Button
	FireRifle,
	// 5 Fire Pistol - Left Mouse Button
	FirePistol
};
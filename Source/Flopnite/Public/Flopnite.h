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
    // 3 ChooseWeapon - Q key
	ChooseWeapon,
    // 4 UseWeapon - Left Mouse Button
	UseWeapon
};
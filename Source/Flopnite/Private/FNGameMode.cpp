// Copyright Epic Games, Inc. All Rights Reserved.

#include "FNGameMode.h"
#include "FNCharacter.h"
#include "FNPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AFNGameMode::AFNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/ALSV4_CPP/AdvancedLocomotionV4/Blueprints/CharacterLogic/ALS_CharacterBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerStateClass = AFNPlayerState::StaticClass();
	}
}

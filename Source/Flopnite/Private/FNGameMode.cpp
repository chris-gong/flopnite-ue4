// Copyright Epic Games, Inc. All Rights Reserved.

#include "FNGameMode.h"
#include "FNCharacter.h"
#include "FNPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AFNGameMode::AFNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerStateClass = AFNPlayerState::StaticClass();
	}
}

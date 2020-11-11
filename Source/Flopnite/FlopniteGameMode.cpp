// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlopniteGameMode.h"
#include "FlopniteCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFlopniteGameMode::AFlopniteGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FortniteCloneGameMode.h"
#include "FortniteCloneCharacter.h"
#include "FortniteClonePlayerState.h"
#include "UObject/ConstructorHelpers.h"

AFortniteCloneGameMode::AFortniteCloneGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerStateClass = AFortniteClonePlayerState::StaticClass();
	}
}

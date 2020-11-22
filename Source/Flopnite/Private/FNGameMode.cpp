// Copyright Epic Games, Inc. All Rights Reserved.

#include "FNGameMode.h"
#include "FNPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AFNGameMode::AFNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonPlayerController"));

	if (PlayerPawnBPClass.Class != NULL && PlayerControllerBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = PlayerControllerBPClass.Class;
		PlayerStateClass = AFNPlayerState::StaticClass();
	}
}

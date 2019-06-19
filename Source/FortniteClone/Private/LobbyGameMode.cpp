// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> LobbyPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_LobbyCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> SpectatorPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_SpectatorCharacter"));
	if (LobbyPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = LobbyPawnBPClass.Class;
		//HUDClass = AMainMenuHUD::StaticClass();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "MainMenuHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> SpectatorPawnBPClass(TEXT("/Game/Blueprints/BP_Spectator"));
	if (SpectatorPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = SpectatorPawnBPClass.Class;
		HUDClass = AMainMenuHUD::StaticClass();
	}
}

void AMainMenuGameMode::StartPlay() {
	Super::StartPlay();
	/*APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;*/
	//MyController->SetInputMode(FInputModeUIOnly());
}
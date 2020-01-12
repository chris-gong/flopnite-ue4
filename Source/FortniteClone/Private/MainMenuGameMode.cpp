// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "Engine/Engine.h"
#include "MainMenuHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> SpectatorPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_MenuCharacter"));
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

void AMainMenuGameMode::PostLogin(APlayerController *NewPlayer) {
	Super::PostLogin(NewPlayer);
	/*FString MouseCursor = FString("spectator tick");
	FString ClickEventsEnabled = FString("spectator tick");
	FString MouseOverEnabled = FString("spectator tick");
	if (NewPlayer->bShowMouseCursor) {
		MouseCursor = FString("Mouse cursor yes");
	}
	else {
		MouseCursor = FString("Mouse cursor no");
	}
	if (NewPlayer->bEnableClickEvents) {
		ClickEventsEnabled = FString("Click events yes");
	}
	else {
		ClickEventsEnabled = FString("Click events no");
	}
	if (NewPlayer->bEnableMouseOverEvents) {
		MouseOverEnabled = FString("Mouse over yes");
	}
	else {
		MouseOverEnabled = FString("Mouse over no");
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, MouseCursor);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ClickEventsEnabled);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, MouseOverEnabled);*/

	/*NewPlayer->bShowMouseCursor = true;
	NewPlayer->bEnableClickEvents = true;
	NewPlayer->bEnableMouseOverEvents = true;
	NewPlayer->SetInputMode(FInputModeGameAndUI());*/
}
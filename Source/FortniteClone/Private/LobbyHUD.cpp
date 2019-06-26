// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"

ALobbyHUD::ALobbyHUD()
{

}

void ALobbyHUD::DrawHUD()
{
	Super::DrawHUD();
}


void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		// change the flags back to their default values after going from main menu to lobby
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine.h"

AMainMenuHUD::AMainMenuHUD()
{
	
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuObj(TEXT("/Game/UI/Widgets/UI_MainMenu"));
	MainMenuWidgetClass = MainMenuObj.Class;
}

void AMainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}


void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
	DrawGameUI();
}

void AMainMenuHUD::DrawGameUI() {
	// remove main screen widget
	/*if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
	}*/
	if (MainMenuWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
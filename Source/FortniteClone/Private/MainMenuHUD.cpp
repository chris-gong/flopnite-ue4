// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

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
		/*FString MouseCursor = FString("spectator tick");
		FString ClickEventsEnabled = FString("spectator tick");
		FString MouseOverEnabled = FString("spectator tick");
		if (PlayerController->bShowMouseCursor) {
			MouseCursor = FString("Mouse cursor yes");
		}
		else {
			MouseCursor = FString("Mouse cursor no");
		}
		if (PlayerController->bEnableClickEvents) {
			ClickEventsEnabled = FString("Click events yes");
		}
		else {
			ClickEventsEnabled = FString("Click events no");
		}
		if (PlayerController->bEnableMouseOverEvents) {
			MouseOverEnabled = FString("Mouse over yes");
		}
		else {
			MouseOverEnabled = FString("Mouse over no");
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, MouseCursor);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ClickEventsEnabled);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, MouseOverEnabled);*/
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
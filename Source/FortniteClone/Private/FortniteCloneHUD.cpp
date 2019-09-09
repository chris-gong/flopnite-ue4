// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteCloneHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

AFortniteCloneHUD::AFortniteCloneHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairObj(TEXT("/Game/UI/Foundation/Textures/Player/T_Crosshair"));
	CrosshairTexture = CrosshairObj.Object;
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/UI/Widgets/UI_Health"));
	HealthWidgetClass = HealthBarObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> MaterialBarObj(TEXT("/Game/UI/Widgets/UI_Materials"));
	MaterialsWidgetClass = MaterialBarObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> ItemBarObj(TEXT("/Game/UI/Widgets/UI_Items"));
	ItemsWidgetClass = ItemBarObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> KillBarObj(TEXT("/Game/UI/Widgets/UI_KillCount"));
	KillsWidgetClass = KillBarObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> HitMarkerObj(TEXT("/Game/UI/Widgets/UI_HitMarker"));
	HitMarkerWidgetClass = HitMarkerObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuObj(TEXT("/Game/UI/Widgets/UI_MainMenu"));
	MainMenuWidgetClass = MainMenuObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> CountObj(TEXT("/Game/UI/Widgets/UI_RemainingPlayersCount"));
	CountWidgetClass = CountObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> BloodEffectObj(TEXT("/Game/UI/Widgets/UI_BloodEffect"));
	BloodEffectWidgetClass = BloodEffectObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> SettingsMenuObj(TEXT("/Game/UI/Widgets/UI_SettingsMenu"));
	SettingsMenuWidgetClass = SettingsMenuObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> BuildingHotkeysObj(TEXT("/Game/UI/Widgets/UI_BuildingHotkeys"));
	BuildingHotkeysWidgetClass = BuildingHotkeysObj.Class;
}

void AFortniteCloneHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrosshair();
}

void AFortniteCloneHUD::BeginPlay()
{
	Super::BeginPlay();
	/*if (MainMenuWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}*/
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		// change the flags back to their default values after going from main menu to fortnite clone
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false; 
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	DrawGameUI();
}

void AFortniteCloneHUD::DrawCrosshair() {
	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X - CrosshairTexture->GetSizeX() / 2),
		(Center.Y - CrosshairTexture->GetSizeY() / 2));

	// draw the crosshair
	FCanvasTileItem CrosshairTileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	CrosshairTileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(CrosshairTileItem);
}

void AFortniteCloneHUD::DrawHitMarker() {
	if (HitMarkerWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HitMarkerWidgetClass);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "hitmarkerclass is not null");
		if (CurrentWidget)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "current widget is not null");
			CurrentWidget->AddToViewport();
			// TODO for later: Implement your own UUserWidget class to retrieve animation from a blueprint and play the animation here
		}
	}
}

void AFortniteCloneHUD::DrawBloodSplash() {
	if (BloodEffectWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), BloodEffectWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			// TODO for later: Implement your own UUserWidget class to retrieve animation from a blueprint and play the animation here
		}
	}
}
void AFortniteCloneHUD::DrawGameUI() {
	// remove main screen widget
	/*if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
	}*/
	if (HealthWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (MaterialsWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MaterialsWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (ItemsWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), ItemsWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (KillsWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), KillsWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (CountWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), CountWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
	if (BuildingHotkeysWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), BuildingHotkeysWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AFortniteCloneHUD::DrawSettingsMenu() {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		// change the flags back to their default values after going from settings menu to fortnite clone
		// or change the flags to ui mode if going from fortnite clone to settings menu
		if (PlayerController->bShowMouseCursor) {
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false; 
			PlayerController->SetInputMode(FInputModeGameOnly());
			if (SettingsMenuWidget) {
				SettingsMenuWidget->RemoveFromViewport();
			}
		}
		else {
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			PlayerController->SetInputMode(FInputModeGameAndUI());
			if (SettingsMenuWidgetClass != nullptr)
			{
				SettingsMenuWidget = CreateWidget<UUserWidget>(GetWorld(), SettingsMenuWidgetClass);

				if (SettingsMenuWidget)
				{
					SettingsMenuWidget->AddToViewport();
				}
			}
		}
	}
}
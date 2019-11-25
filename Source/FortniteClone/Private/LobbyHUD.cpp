// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

ALobbyHUD::ALobbyHUD()
{
	// copied from FortniteCloneHUD class
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairObj(TEXT("/Game/UI/Foundation/Textures/Player/T_Crosshair"));
	CrosshairTexture = CrosshairObj.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> CountObj(TEXT("/Game/UI/Widgets/UI_PlayersJoinedCount"));
	PlayersJoinedCountWidgetClass = CountObj.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> WaitingForPlayersObj(TEXT("/Game/UI/Widgets/UI_WaitingForPlayers"));
	WaitingForPlayersWidgetClass = WaitingForPlayersObj.Class;
}

void ALobbyHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrosshair();
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
	DrawGameUI();
}

void ALobbyHUD::DrawCrosshair() {
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

void ALobbyHUD::DrawGameUI(){
	if (PlayersJoinedCountWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayersJoinedCountWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (WaitingForPlayersWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WaitingForPlayersWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
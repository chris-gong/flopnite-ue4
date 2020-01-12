// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class UUserWidget;
class UTexture2D;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALobbyHUD();

	/* Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	void DrawCrosshair();

	void DrawGameUI();
private:
	UTexture2D* CrosshairTexture;

	UPROPERTY(EditAnywhere, Category = "PlayersJoinedCount")
		TSubclassOf<UUserWidget> PlayersJoinedCountWidgetClass;
	UPROPERTY(EditAnywhere, Category = "WaitingForPlayers")
		TSubclassOf<UUserWidget> WaitingForPlayersWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
		UUserWidget* CurrentWidget;
};

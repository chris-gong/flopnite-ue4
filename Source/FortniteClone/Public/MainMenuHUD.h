// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMainMenuHUD();

	/* Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	void DrawGameUI();

private:
	UPROPERTY(EditAnywhere, Category = "MainMenu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UUserWidget* CurrentWidget;
};

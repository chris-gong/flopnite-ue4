// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FortniteCloneHUD.generated.h"

class UUserWidget;
class UTexture2D;

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AFortniteCloneHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AFortniteCloneHUD();

	/* Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	/* Crosshair Asset */
	UTexture2D* CrosshairTexture;

	UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Materials")
	TSubclassOf<UUserWidget> MaterialsWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<UUserWidget> ItemsWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UUserWidget* CurrentWidget;
};

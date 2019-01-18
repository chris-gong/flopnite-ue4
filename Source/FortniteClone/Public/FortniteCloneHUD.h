// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FortniteCloneHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AFortniteCloneHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AFortniteCloneHUD();

	/** Primary draw call for the HUD */
	//virtual void DrawHUD() override;

	//virtual void BeginPlay() override;

private:
	/*UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
	UUserWidget* CurrentWidget;*/
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteCloneHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AFortniteCloneHUD::AFortniteCloneHUD()
{

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/UI/Health_UI"));
	HUDWidgetClass = HealthBarObj.Class;
}

void AFortniteCloneHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AFortniteCloneHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
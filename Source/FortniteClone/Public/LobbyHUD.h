// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

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
};

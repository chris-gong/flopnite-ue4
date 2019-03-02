// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FortniteClonePlayerController.generated.h"

class AFortniteCloneSpectator;
class AStormActor;
/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AFortniteClonePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFortniteClonePlayerController();

	virtual void Tick(float DeltaTime) override;

	void SwitchToSpectatorMode();

	bool Initialized;

	AStormActor* CurrentStorm;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	TSubclassOf<AFortniteCloneSpectator> PlayerSpectatorClass;

private:
	virtual void BeginPlay() override;
};

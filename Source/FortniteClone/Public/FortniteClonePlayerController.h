// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FortniteClonePlayerController.generated.h"

class AFortniteCloneSpectator;
class AStormActor;
class AGameMode;
class AFortniteClonePlayerState;
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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSwitchToSpectatorMode();

	bool Initialized;

	FString PlayerSessionId;

	AStormActor* CurrentStorm;

	TSubclassOf<AFortniteCloneSpectator> PlayerSpectatorClass;

	UPROPERTY(Replicated)
	int PlayerCount;

	UPROPERTY(Replicated)
	int SpectatorCount;

	UFUNCTION(BlueprintPure, Category = "Count")
	int GetPlayerCount();
		
	UFUNCTION(BlueprintPure, Category = "Count")
	int GetSpectatorCount();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetNumPlayers();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetNumSpectators();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateCountAfterDeath();

	UFUNCTION(BlueprintPure, Category = "Count")
	int GetKillCount();

	UPROPERTY(Replicated)
	bool SpawnAsSpectator;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

private:
	virtual void BeginPlay() override;

};

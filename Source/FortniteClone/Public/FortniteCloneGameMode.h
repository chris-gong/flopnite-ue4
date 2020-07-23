// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FortniteCloneGameMode.generated.h"

class AStormActor;

DECLARE_LOG_CATEGORY_EXTERN(LogMyServerGame, Log, All);

UCLASS(minimalapi)
class AFortniteCloneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFortniteCloneGameMode();

	bool GameStarted; 

	bool GameEnded;

	int TimePassed;

<<<<<<< HEAD
	UPROPERTY(BlueprintReadOnly)
		 class AFortniteCloneCharacter * PlayerKiller;

	UPROPERTY(BlueprintReadOnly)
		class AFortniteCloneCharacter * PlayerKilled;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Storm | DO NOT CHANGE ")
=======
>>>>>>> 8291d0bfd62b9a8353bd9f60c662263c1893b6a9
	AStormActor* CurrentStorm;

	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController *NewPlayer) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void Logout(AController* Exiting) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;

	virtual void GenericPlayerInitialization(AController* NewPlayer) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSpawnStorm();

	UFUNCTION(Server, Reliable, WithValidation)
	void GameModeStartStorm();

	UFUNCTION(Server, Reliable, WithValidation)
	void CheckRemainingPlayers();
};




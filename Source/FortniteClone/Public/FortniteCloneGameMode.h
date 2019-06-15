// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameLiftServerSDK.h"
//#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
//#include "GameLiftClientSDK/Public/GameLiftClientApi.h"
#include "FortniteCloneGameMode.generated.h"

class AStormActor;

DECLARE_LOG_CATEGORY_EXTERN(LogMyServer, Log, All);

UCLASS(minimalapi)
class AFortniteCloneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFortniteCloneGameMode();

	bool Initialized; 

	AStormActor* CurrentStorm;

	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController *NewPlayer) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSpawnStorm();

	UFUNCTION(Server, Reliable, WithValidation)
	void GameModeStartStorm();

	int TimeSinceInitialization;

	UFUNCTION(Server, Reliable, WithValidation)
	void TickInitializationClock();
};




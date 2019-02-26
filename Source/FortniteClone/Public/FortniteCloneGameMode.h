// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FortniteCloneGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyServer, Log, All);

UCLASS(minimalapi)
class AFortniteCloneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFortniteCloneGameMode();

	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController *NewPlayer) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSpawnStorm();
};




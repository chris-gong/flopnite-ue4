// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FortniteCloneGameMode.h"
#include "FortniteCloneCharacter.h"
#include "FortniteClonePlayerState.h"
#include "FortniteClonePlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameLiftServerSDK.h"
#include "FortniteCloneHUD.h"
#include "StormActor.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "FortGameState.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/GameState.h"
//#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
//#include "GameLiftClientSDK/Public/GameLiftClientApi.h"

DEFINE_LOG_CATEGORY(LogMyServerGame);

AFortniteCloneGameMode::AFortniteCloneGameMode()
{
	// set default pawn class to our Blueprinted character
	GameStarted = false;
	GameEnded = false;
	TimePassed = 0;
	bUseSeamlessTravel = true;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> SpectatorPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_SpectatorCharacter"));
	if (PlayerPawnBPClass.Class != NULL && SpectatorPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		SpectatorClass = SpectatorPawnBPClass.Class;
		PlayerStateClass = AFortniteClonePlayerState::StaticClass();
		PlayerControllerClass = AFortniteClonePlayerController::StaticClass();
		HUDClass = AFortniteCloneHUD::StaticClass();
	}
}

void AFortniteCloneGameMode::BeginPlay() {
	Super::BeginPlay();

}

void AFortniteCloneGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AFortniteCloneGameMode::DrawNewEye() {
	
}

void AFortniteCloneGameMode::StartPlay() {
	Super::StartPlay();

}

void AFortniteCloneGameMode::PostLogin(APlayerController *NewPlayer) {
	Super::PostLogin(NewPlayer);
	Players.Add(Cast<AFortniteClonePlayerController>(NewPlayer));
}

void AFortniteCloneGameMode::PlayerDied(class AFortniteCloneCharacter * Killed, class AFortniteCloneCharacter * Killer)
{
	if (Killed)
	{
		PlayerKilled = Killed;

		PlayerKiller = Killer;

		if (AFortniteClonePlayerController * PC = Cast<AFortniteClonePlayerController>(Killed->GetController()))
		{
			Players.RemoveSingle(PC);
		}
		if (Players.Num() == 1 && Players.IsValidIndex(0))
		{
			WinnerFound(Cast<AFortniteClonePlayerState>(Players[0]->PlayerState));
		}
	}
}

void AFortniteCloneGameMode::WinnerFound(class AFortniteClonePlayerState* WinnerState)
{
	if (AFortGameState* GS = GetGameState<AFortGameState>())
	{
		GS->Winner = WinnerState;
	}
}


void AFortniteCloneGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) {
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AFortniteCloneGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);
	if (Exiting != nullptr) {
		AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(Exiting);
		const FString& PlayerSessionId = FortniteClonePlayerController->PlayerSessionId;
		UE_LOG(LogMyServerGame, Log, TEXT("Logout playersessionid: %s"), *PlayerSessionId);
		if (PlayerSessionId.Len() > 0) {
#if WITH_GAMELIFT
			FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
			FGameLiftGenericOutcome outcome = gameLiftSdkModule->RemovePlayerSession(PlayerSessionId);
			UE_LOG(LogMyServerGame, Log, TEXT("FortniteGameMode::Logout: Removing Client with GameLift PlayerSessionId: %s"), *PlayerSessionId);
			if (!outcome.IsSuccess())
			{
				const FString ErrorMessage = outcome.GetError().m_errorMessage;
				UE_LOG(LogMyServerGame, Log, TEXT("FortniteGameMode::Logout:  Removing Client invalid GameLift PlayerSessionId: %s, Error: %s"), *PlayerSessionId, **ErrorMessage);
			}
#endif
		}
	}
}

FString AFortniteCloneGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) {
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	
	/*const FString& PlayerSessionId = UGameplayStatics::ParseOption(Options, "PlayerSessionId");
	UE_LOG(LogMyServerGame, Log, TEXT("InitNewPlayer playersessionid: %s"), *PlayerSessionId);
	if (NewPlayerController != nullptr) {
		AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(NewPlayerController);
		FortniteClonePlayerController->PlayerSessionId = PlayerSessionId;
	}*/

	return InitializedString;
}

void AFortniteCloneGameMode::GenericPlayerInitialization(AController* NewPlayer) {
	Super::GenericPlayerInitialization(NewPlayer);
	if (!GameStarted && GetNumPlayers() >= 2) {
		GameStarted = true;
		TArray<AActor*> StormActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStormActor::StaticClass(), StormActors);
		if (StormActors.Num() > 0) {
			if (StormActors[0] != nullptr) {
				CurrentStorm = Cast<AStormActor>(StormActors[0]);
			}
		}
		FTimerHandle StormSetupTimerHandle;
		GetWorldTimerManager().SetTimer(StormSetupTimerHandle, this, &AFortniteCloneGameMode::GameModeStartStorm, 30.0f, false);
		FTimerHandle InactivityCheckTimerHandle;
		GetWorldTimerManager().SetTimer(InactivityCheckTimerHandle, this, &AFortniteCloneGameMode::CheckRemainingPlayers, 1.0f, true);
	}
	if (NewPlayer != nullptr) {
		AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(NewPlayer);
		FortniteClonePlayerController->SpawnAsSpectator = false; // since we have switched to server travel, everyone joining the game should be a player
	}
}

void AFortniteCloneGameMode::NetMulticastSpawnStorm_Implementation() {
	GetWorld()->SpawnActor<AStormActor>(AStormActor::StaticClass(), FVector(-440, -1450, 10000), FRotator::ZeroRotator);
}

void AFortniteCloneGameMode::GameModeStartStorm_Implementation() {
	if (CurrentStorm != nullptr) {
		CurrentStorm->ServerStartStorm();
	}
	else
	{
		CurrentStorm = GetWorld()->SpawnActor<AStormActor>(AStormActor::StaticClass(), FVector(-440, -1450, 10000), FRotator::ZeroRotator);
		CurrentStorm->ServerStartStorm();
		return;

	}
	
}

bool AFortniteCloneGameMode::GameModeStartStorm_Validate() {
	return true;
}

void AFortniteCloneGameMode::CheckRemainingPlayers_Implementation() {
	AGameStateBase* FortniteCloneGameState = Cast<AGameStateBase>(GetWorld()->GetGameState<AGameStateBase>());
	if (FortniteCloneGameState) {
		//UE_LOG(LogMyServerGame, Log, TEXT("Game state exists"));
		int NumPlayers = 0;
		TArray<APlayerState*> States = FortniteCloneGameState->PlayerArray;
		for (int i = 0; i < States.Num(); i++) {
			if (States[i]->bIsSpectator == false) {
				NumPlayers++;
			}
		}
		if (!GameEnded && GameStarted && NumPlayers < 2) {
			// the game is over, terminate the game session
			TimePassed += 1;
			if (TimePassed >= 15) {
				//UE_LOG(LogMyServerGame, Log, TEXT("Game ended"));
#if WITH_GAMELIFT
				FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
				FGameLiftGenericOutcome outcome = gameLiftSdkModule->ProcessEnding();
				UE_LOG(LogMyServerGame, Log, TEXT("FortniteCloneGameMode::EndGame"));
				if (!outcome.IsSuccess())
				{
					const FString ErrorMessage = outcome.GetError().m_errorMessage;
					UE_LOG(LogMyServerGame, Log, TEXT("FortniteCloneGameMode::EndGame: Error: %s"), *ErrorMessage);
				}
				else {
					FGenericPlatformMisc::RequestExit(false);
				}
#endif
				GameEnded = true;
			}
		}
		else if (!GameEnded && NumPlayers >= 2) {
			TimePassed = 0;
		}
	}
	else {
		//UE_LOG(LogMyServerGame, Log, TEXT("Game state does not exist"));
	}
}

bool AFortniteCloneGameMode::CheckRemainingPlayers_Validate() {
	return true;

}

void AFortniteCloneGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)  const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}


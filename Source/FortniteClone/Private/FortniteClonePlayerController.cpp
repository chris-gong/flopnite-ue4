// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerController.h"
#include "FortniteClonePlayerState.h"
#include "FortniteCloneCharacter.h"
#include "FortniteCloneSpectator.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "StormActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AFortniteClonePlayerController::AFortniteClonePlayerController() {
	/*AFortniteClonePlayerState* State= Cast<AFortniteClonePlayerState>(GetPlayerState());
	if (State) {
		State->bIsSpectator = true;
	}*/
	//PlayerState->bIsSpectator = true;
	 static ConstructorHelpers::FClassFinder<AFortniteCloneSpectator> PlayerSpectatorBP(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_SpectatorCharacter"));
	 PlayerSpectatorClass = PlayerSpectatorBP.Class;
	 PlayerCount = 0;
	 SpectatorCount = 1;
	 Initialized = false;
	 PlayerSessionId = FString("");
}

void AFortniteClonePlayerController::BeginPlay() {
	Super::BeginPlay();
	if (HasAuthority()) {
		TArray<AActor*> StormActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStormActor::StaticClass(), StormActors);
		if (StormActors.Num() > 0) {
			if (StormActors[0] != nullptr) {
				CurrentStorm = Cast<AStormActor>(StormActors[0]);
			}
		}
	}
}

void AFortniteClonePlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (HasAuthority()) {
		if (SpawnAsSpectator && PlayerState && !PlayerState->bIsSpectator) {
			ChangeState(NAME_Spectating);
			ClientGotoState(NAME_Spectating);
			APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor<AFortniteCloneSpectator>(PlayerSpectatorClass, FVector(-900, 350.0, 31812), FRotator::ZeroRotator));
			//SetSpectatorPawn(Pawn);
			Possess(PlayerPawn);
			Cast<AFortniteClonePlayerState>(PlayerState)->bIsSpectator = true; // ORDER MATTERS HERE, HAS TO BE SET AFTER POSSESSING A PAWN
		}
	}
}

void AFortniteClonePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFortniteClonePlayerController, PlayerCount);
	DOREPLIFETIME(AFortniteClonePlayerController, SpectatorCount);
}

void AFortniteClonePlayerController::ServerSwitchToSpectatorMode_Implementation() {
	if (PlayerState) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Player state is not null"));
		//Cast<AFortniteClonePlayerState>(PlayerState)->bIsSpectator = true;
		ChangeState(NAME_Spectating);
		ClientGotoState(NAME_Spectating);
		APawn* PlayerPawn = Cast<APawn>(GetWorld()->SpawnActor<AFortniteCloneSpectator>(PlayerSpectatorClass, FVector(-900, 350.0, 31812), FRotator::ZeroRotator));
		//SetSpectatorPawn(Pawn);
		Possess(PlayerPawn);
		Cast<AFortniteClonePlayerState>(PlayerState)->bIsSpectator = true; // ORDER MATTERS HERE, HAS TO BE SET AFTER POSSESSING A PAWN
		/*FString LogMsg = FString("switch to spectator mode ") + FString::FromInt(GetNetMode());
		UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
		LogMsg = FString("test ") + FString::FromInt(GetNetMode());
		UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
		if (Cast<AFortniteClonePlayerState>(PlayerState)->bIsSpectator) {
			/*LogMsg = FString("What a spectator");
			UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
		}
		else {
			/*LogMsg = FString("What not a spectator");
			UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
		}
	}
}

bool AFortniteClonePlayerController::ServerSwitchToSpectatorMode_Validate() {
	return true;
}

void AFortniteClonePlayerController::ServerGetNumPlayers_Implementation() {
	int Count = 0;
	//FLocalPlayerContext Context(this);
	//UWorld* World = Context.GetWorld();
	/*AGameState* GameState = Cast<AGameState>(GetWorld()->GetGameState());
	if (GameState) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Gamestate is not null"));
		TArray<APlayerState*> States = GameState->PlayerArray;
		for (int i = 0; i < States.Num(); i++) {
			if (States[i]->bIsSpectator == false) {
				Count++;
			}
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Gamestate is null"));
	}
	PlayerCount = Count;*/
}

bool AFortniteClonePlayerController::ServerGetNumPlayers_Validate() {
	return true;
}

void AFortniteClonePlayerController::ServerGetNumSpectators_Implementation() {
	int Count = 0;
	//FLocalPlayerContext Context(this);
	//UWorld* World = Context.GetWorld();
	/*AGameState* GameState = Cast<AGameState>(GetWorld()->GetGameState);
	if (GameState) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Gamemode is not null"));
		TArray<APlayerState*> States = GameState->PlayerArray;
		for (int i = 0; i < States.Num(); i++) {
			if (States[i]->bIsSpectator == true) {
				Count++;
			}
		}
	}
	SpectatorCount = Count;*/
}

bool AFortniteClonePlayerController::ServerGetNumSpectators_Validate() {
	return true;
}

void AFortniteClonePlayerController::ServerUpdateCountAfterDeath_Implementation() {
	
}

bool AFortniteClonePlayerController::ServerUpdateCountAfterDeath_Validate() {
	return true;
}

int AFortniteClonePlayerController::GetKillCount() {
	if (PlayerState) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(PlayerState);
		if (State) {
			return State->KillCount;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int AFortniteClonePlayerController::GetPlayerCount() {
	ServerGetNumPlayers();
	return PlayerCount;
}

int AFortniteClonePlayerController::GetSpectatorCount() {
	ServerGetNumSpectators();
	return SpectatorCount;
}
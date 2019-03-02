// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteClonePlayerController.h"
#include "FortniteClonePlayerState.h"
#include "FortniteCloneCharacter.h"
#include "FortniteCloneSpectator.h"
#include "GameFramework/PlayerState.h"
#include "Engine.h"
#include "StormActor.h"

AFortniteClonePlayerController::AFortniteClonePlayerController() {
	/*AFortniteClonePlayerState* State= Cast<AFortniteClonePlayerState>(GetPlayerState());
	if (State) {
		State->bIsSpectator = true;
	}*/
	//PlayerState->bIsSpectator = true;
	 static ConstructorHelpers::FClassFinder<AFortniteCloneSpectator> PlayerSpectatorBP(TEXT("/Game/Blueprints/BP_Spectator"));
	 PlayerSpectatorClass = PlayerSpectatorBP.Class;
}

void AFortniteClonePlayerController::BeginPlay() {
	Super::BeginPlay();
	if (HasAuthority()) {
		Initialized = false;
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
		if (!Initialized && PlayerState && !PlayerState->bIsSpectator) {
			Initialized = true;
			float TimeSinceGameStarted = 0;
			TimeSinceGameStarted = CurrentStorm->GetGameTimeSinceCreation();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Player controller player state exists in begin play ") + FString::SanitizeFloat(TimeSinceGameStarted));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(TimeSinceGameStarted));
			/* if the game has been going for more than 2 and a half minutes, automatically make the player a spectator*/
			if (TimeSinceGameStarted >= 150) {
				PlayerState->bIsSpectator = true;
				ChangeState(NAME_Spectating);
				ClientGotoState(NAME_Spectating);
				APawn* Pawn = Cast<APawn>(GetWorld()->SpawnActor<AFortniteCloneSpectator>(PlayerSpectatorClass, FVector(-900, 350.0, 31812), FRotator::ZeroRotator));
				//SetSpectatorPawn(Pawn);
				Possess(Pawn);
			}
		}
	}
}

void AFortniteClonePlayerController::SwitchToSpectatorMode() {
	if (PlayerState) {
		PlayerState->bIsSpectator = true;
		ChangeState(NAME_Spectating);
		ClientGotoState(NAME_Spectating);
		APawn* Pawn = Cast<APawn>(GetWorld()->SpawnActor<AFortniteCloneSpectator>(PlayerSpectatorClass, FVector(-900, 350.0, 31812), FRotator::ZeroRotator));
		//SetSpectatorPawn(Pawn);
		Possess(Pawn);
		FString LogMsg = FString("switch to spectator mode ") + FString::FromInt(GetNetMode());
		UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
	}
}
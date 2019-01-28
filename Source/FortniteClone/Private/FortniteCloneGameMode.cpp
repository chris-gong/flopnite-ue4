// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FortniteCloneGameMode.h"
#include "FortniteCloneCharacter.h"
#include "FortniteClonePlayerState.h"
#include "FortniteCloneHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "FortniteClone.h"

AFortniteCloneGameMode::AFortniteCloneGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerStateClass = AFortniteClonePlayerState::StaticClass();
		HUDClass = AFortniteCloneHUD::StaticClass();
	}

}

void AFortniteCloneGameMode::StartPlay() {
	Super::StartPlay();
	//UGameplayStatics::OpenLevel((UObject*)GetWorld(), FName(TEXT("Level_BattleRoyale")));
}

void AFortniteCloneGameMode::PostLogin(APlayerController *NewPlayer) {
	Super::PostLogin(NewPlayer);
	//set input to ui only for the main screen level
	/*NewPlayer->SetInputMode(FInputModeGameAndUI());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(NewPlayer->PlayerState);
	if (State) {
		//AFortniteCloneCharacter* Character = Cast<AFortniteCloneCharacter>(NewPlayer->GetPawn());
		//Character->State = State;
		State->HoldingWeapon = true;
		State->CurrentWeapon = 0;
	}*/
}
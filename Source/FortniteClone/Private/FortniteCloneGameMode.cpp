// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FortniteCloneGameMode.h"
#include "FortniteCloneCharacter.h"
#include "FortniteClonePlayerState.h"
#include "FortniteCloneHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "FortniteClone.h"
#include "GameLiftServerSDK.h"
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#include "GameLiftClientSDK/Public/GameLiftClientApi.h"

DEFINE_LOG_CATEGORY(LogMyServer);

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

#/*if WITH_GAMELIFTCLIENTSDK
	// Create the game lift object. This is required before calling any GameLift functions.
	GameLiftClientObject = UGameLiftClientObject::CreateGameLiftObject("Your Access Key", "Your Secret Key");
	
	bool includeBrackets = false;
	bool includeDashes = true;
	string playerId = AZ::Uuid::CreateRandom().ToString<string>(includeBrackets, includeDashes);
#endif*/

	//Let's run this code only if GAMELIFT is enabled. Only with Server targets!
#if WITH_GAMELIFT

	//Getting the module first.
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	//InitSDK establishes a local connection with GameLift's agent to enable communication.
	gameLiftSdkModule->InitSDK();

	//Respond to new game session activation request. GameLift sends activation request 
	//to the game server along with a game session object containing game properties 
	//and other settings. Once the game server is ready to receive player connections, 
	//invoke GameLiftServerAPI.ActivateGameSession()
	auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		gameLiftSdkModule->ActivateGameSession();
	};

	FProcessParameters* params = new FProcessParameters();
	params->OnStartGameSession.BindLambda(onGameSession);

	//OnProcessTerminate callback. GameLift invokes this before shutting down the instance 
	//that is hosting this game server to give it time to gracefully shut down on its own. 
	//In this example, we simply tell GameLift we are indeed going to shut down.
	params->OnTerminate.BindLambda([=]() {gameLiftSdkModule->ProcessEnding(); });

	//HealthCheck callback. GameLift invokes this callback about every 60 seconds. By default, 
	//GameLift API automatically responds 'true'. A game can optionally perform checks on 
	//dependencies and such and report status based on this info. If no response is received  
	//within 60 seconds, health status is recorded as 'false'. 
	//In this example, we're always healthy!
	params->OnHealthCheck.BindLambda([]() {return true; });

	//Here, the game server tells GameLift what port it is listening on for incoming player 
	//connections. In this example, the port is hardcoded for simplicity. Since active game
	//that are on the same instance must have unique ports, you may want to assign port values
	//from a range, such as:
	//const int32 port = FURL::UrlConfig.DefaultPort;
	//params->port;
	params->port = 7777;

	//Here, the game server tells GameLift what set of files to upload when the game session 
	//ends. GameLift uploads everything specified here for the developers to fetch later.
	TArray<FString> logfiles;
	logfiles.Add(TEXT("aLogFile.txt"));
	params->logParameters = logfiles;

	//Call ProcessReady to tell GameLift this game server is ready to receive game sessions!
	gameLiftSdkModule->ProcessReady(*params);

#endif
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

void AFortniteCloneGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) {
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
#if WITH_GAMELIFT
	FString PlayerSessionId = UniqueId.ToString();

	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	FGameLiftGenericOutcome outcome = gameLiftSdkModule->AcceptPlayerSession(*PlayerSessionId);
	UE_LOG(LogMyServer, Log, TEXT("AMyProjectGameMode::PreLogin: Client connecting with attempting to connect with GameLift PlayerSessionId: %s"), *PlayerSessionId);
	if (!outcome.IsSuccess())
	{
		ErrorMessage = outcome.GetError().m_errorMessage;
		UE_LOG(LogMyServer, Log, TEXT("AMyProjectGameMode::PreLogin: Client connecting with invalid GameLift PlayerSessionId: %s, Error: %s"), *PlayerSessionId, *ErrorMessage);
	}
#endif
}
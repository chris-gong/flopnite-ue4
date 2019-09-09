// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TextReaderComponent.h"
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#include "GameLiftClientSDK/Public/GameLiftClientApi.h"
#include "Engine/Engine.h"
#include "Runtime\Engine\Classes\Kismet\KismetMathLibrary.h"
#include "Runtime/UMG/Public/Components/Button.h"

DEFINE_LOG_CATEGORY(LogMyMainMenu);
// Add default functionality here for any IMainMenuWidget functions that are not pure virtual.
UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));
	//JoinGameButton = (UButton*)GetWidgetFromName(TEXT("Button_JoinGame")); <- have to do this in native construct, not the constructor
	AccessKey = TextReader->ReadFile("Credentials/AWS_AccessKey.txt");
	SecretKey = TextReader->ReadFile("Credentials/AWS_SecretKey.txt");
	QueueName = TextReader->ReadFile("Credentials/AWS_QueueName.txt");

	AttemptToJoinGameFinished = true; // default value true so that we can prevent double clicks
	FailedToJoinGame = false;
	SucceededToJoinGame = false;
	GameSessionsLeft = 4;

	DescribeGameSessionQueuesEvent = FGenericPlatformProcess::GetSynchEventFromPool(false);
	SearchGameSessionsEvent = FGenericPlatformProcess::GetSynchEventFromPool(false);
	CreatePlayerSessionEvent = FGenericPlatformProcess::GetSynchEventFromPool(false);
	StartGameSessionPlacementEvent = FGenericPlatformProcess::GetSynchEventFromPool(false);
	DescribeGameSessionPlacementEvent = FGenericPlatformProcess::GetSynchEventFromPool(false);
}

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();
	// line below is for testing local gamelift
	//Client = UGameLiftClientObject::CreateGameLiftObject(AccessKey, SecretKey, "us-east-2", true, 9080);
	Client = UGameLiftClientObject::CreateGameLiftObject(AccessKey, SecretKey, "us-east-2");
	
	JoinGameButton = (UButton*) GetWidgetFromName(TEXT("Button_JoinGame"));
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinGame);
	ExitGameButton = (UButton*)GetWidgetFromName(TEXT("Button_ExitGame"));
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);
	/*
	LaunchDiscordButton = (UButton*)GetWidgetFromName(TEXT("Button_LaunchDiscord"));
	LaunchDiscordButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LaunchDiscord);
	LaunchYoutubeButton = (UButton*)GetWidgetFromName(TEXT("Button_LaunchYoutube"));
	LaunchYoutubeButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LaunchYoutube);
	LaunchTwitterButton = (UButton*)GetWidgetFromName(TEXT("Button_LaunchTwitter"));
	LaunchTwitterButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LaunchTwitter);
	LaunchPatreonButton = (UButton*)GetWidgetFromName(TEXT("Button_LaunchPatreon"));
	LaunchPatreonButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LaunchPatreon);
	LaunchInstagramButton = (UButton*)GetWidgetFromName(TEXT("Button_LaunchInstagram"));
	LaunchInstagramButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LaunchInstagram);
	LaunchFacebookButton = (UButton*)GetWidgetFromName(TEXT("Button_LaunchFacebook"));
	LaunchFacebookButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LaunchFacebook);
	*/
}
void UMainMenuWidget::JoinGame() {
	AttemptToJoinGameFinished = false;
	JoinGameButton->SetIsEnabled(false);
	DisableMouseEvents();

	FailedToJoinGame = false;
	SucceededToJoinGame = false;

	DescribeGameSessionQueues(QueueName);
	DescribeGameSessionQueuesEvent->Wait();

	if (AttemptToJoinGameFinished) {
		if (SucceededToJoinGame) {
			// don't reenable anything since game was successfully joined
		}
		else if (FailedToJoinGame) {
			JoinGameButton->SetIsEnabled(true);
			EnableMouseEvents();
		}
		return;
	}

	const FString& PlacementId = GenerateRandomId();
	StartGameSessionPlacement(QueueName, 100, PlacementId);
	StartGameSessionPlacementEvent->Wait();

	AttemptToJoinGameFinished = true;
	if (SucceededToJoinGame) {
		// don't reenable anything since game was successfully joined
	}
	else if (FailedToJoinGame) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString("No game sessions available currently"));
		JoinGameButton->SetIsEnabled(true);
		EnableMouseEvents();
	}
}

void UMainMenuWidget::DescribeGameSessionQueues(const FString& QueueNameInput) {
	UGameLiftDescribeGameSessionQueues* DescribeGameSessionQueuesObject = Client->DescribeGameSessionQueues(QueueNameInput);
	DescribeGameSessionQueuesObject->OnDescribeGameSessionQueuesSuccess.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionQueuesSuccess);
	DescribeGameSessionQueuesObject->OnDescribeGameSessionQueuesFailed.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionQueuesFailed);
	DescribeGameSessionQueuesObject->Activate();
	//UE_LOG(LogMyMainMenu, Log, TEXT("describe game session queues activated"));
}

void UMainMenuWidget::OnDescribeGameSessionQueuesSuccess(const TArray<FString>& FleetARNs) {
	//UE_LOG(LogMyMainMenu, Log, TEXT("describe game session queues success"));
	for (int i = 0; i < FleetARNs.Num(); i++) {
		FString FleetArn = FleetARNs[i];
		//UE_LOG(LogMyMainMenu, Log, TEXT("%s"), *FleetArn);
		TArray<FString> FleetArnParsedOnce;
		FleetArn.ParseIntoArray(FleetArnParsedOnce, TEXT("arn:aws:gamelift:"), true);
		TArray<FString> FleetArnParsedAgain;
		FleetArnParsedOnce[0].ParseIntoArray(FleetArnParsedAgain, TEXT("::fleet/"), true);

		const FString& FleetId = FleetArnParsedAgain[1];
		SearchGameSessions(FleetId);
		SearchGameSessionsEvent->Wait();

		if (AttemptToJoinGameFinished) {
			break;
		}
	}
	DescribeGameSessionQueuesEvent->Trigger();
}

void UMainMenuWidget::OnDescribeGameSessionQueuesFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session queues failed %s"), *ErrorMessage);
	DescribeGameSessionQueuesEvent->Trigger();
}

void UMainMenuWidget::SearchGameSessions(const FString& FleetId) {
	UGameLiftSearchGameSessions* SearchGameSessionsObject = Client->SearchGameSessions(FleetId, "", "", ""); // TODO: Have to handle other parameters
	SearchGameSessionsObject->OnSearchGameSessionsSuccess.AddDynamic(this, &UMainMenuWidget::OnSearchGameSessionsSuccess);
	SearchGameSessionsObject->OnSearchGameSessionsFailed.AddDynamic(this, &UMainMenuWidget::OnSearchGameSessionsFailed);
	SearchGameSessionsObject->Activate();
}
void UMainMenuWidget::OnSearchGameSessionsSuccess(const TArray<FString>& GameSessionIds) {
	//SearchGameSessionsFinished = true;
	//UE_LOG(LogMyMainMenu, Log, TEXT("on search game session success"));
	//GameSessionsLeft = GameSessionIds.Num();

	/*if (GameSessionsLeft <= 0) {
		AttemptToJoinGameFinished = true;

		const FString& PlacementId = GenerateRandomId();
		StartGameSessionPlacement(QueueName, 100, PlacementId);
	}*/
	//else {
	for (int i = 0; i < GameSessionIds.Num(); i++) {
		const FString& GameSessionId = GameSessionIds[i];
		const FString& PlayerSessionId = GenerateRandomId();
		//UE_LOG(LogMyMainMenu, Log, TEXT("on search game session success Game session id %s"), *GameSessionId);
		CreatePlayerSession(GameSessionId, PlayerSessionId);
		CreatePlayerSessionEvent->Wait();

		if (AttemptToJoinGameFinished) {
			break;
		}
	}
	//}
	SearchGameSessionsEvent->Trigger();
}

void UMainMenuWidget::OnSearchGameSessionsFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on search game sessions failed %s"), *ErrorMessage);
	/*AttemptToJoinGameFinished = true;
	JoinGameButton->SetIsEnabled(true);*/
	SearchGameSessionsEvent->Trigger();
}

void UMainMenuWidget::CreatePlayerSession(const FString& GameSessionId, const FString& PlayerSessionId) {
	UGameLiftCreatePlayerSession* CreatePlayerSessionObject = Client->CreatePlayerSession(GameSessionId, PlayerSessionId);
	CreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UMainMenuWidget::OnCreatePlayerSessionSuccess);
	CreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UMainMenuWidget::OnCreatePlayerSessionFailed);
	CreatePlayerSessionObject->Activate();
}

void UMainMenuWidget::OnCreatePlayerSessionSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID, const int& PlayerSessionStatus) {
	FString PlayerSessionStatusString = FString::FromInt(PlayerSessionStatus);
	//UE_LOG(LogMyMainMenu, Log, TEXT("player session status %s"), *PlayerSessionStatusString);
	if (PlayerSessionStatus == 1) {
		FString LevelName = IPAddress + FString(":") + Port;
		const FString& Options = FString("?") + FString("PlayerSessionId=") + PlayerSessionID;

		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), false, Options);

		AttemptToJoinGameFinished = true;
		SucceededToJoinGame = true;
		FailedToJoinGame = false;
	}
	else if (PlayerSessionStatus == 2) {
		// already activated?
	}
	else {
		// pending or timed out?
	}
	CreatePlayerSessionEvent->Trigger();
}

void UMainMenuWidget::OnCreatePlayerSessionFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on create player session failed %s"), *ErrorMessage);
	CreatePlayerSessionEvent->Trigger();
}

void UMainMenuWidget::StartGameSessionPlacement(const FString& QueueNameInput, const int& MaxPlayerCount, const FString& PlacementId) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("start game session placement called in main menu widget"));
	UGameLiftStartGameSessionPlacement* StartGameSessionPlacementObject = Client->StartGameSessionPlacement(QueueNameInput, MaxPlayerCount, PlacementId);
	StartGameSessionPlacementObject->OnStartGameSessionPlacementSuccess.AddDynamic(this, &UMainMenuWidget::OnStartGameSessionPlacementSuccess);
	StartGameSessionPlacementObject->OnStartGameSessionPlacementFailed.AddDynamic(this, &UMainMenuWidget::OnStartGameSessionPlacementFailed);
	StartGameSessionPlacementObject->Activate();
}

void UMainMenuWidget::OnStartGameSessionPlacementSuccess(const FString& GameSessionId, const FString& PlacementId, const int& Status) {
	if (Status == 0 && GameSessionId.Len() <= 0) {
		for (int i = 0; i < 10; i++) {
			// check on game session placement 10 times, or until it's state is fulfilled and id is made
			StartGameSessionPlacementEvent->Wait(500); // wait 500 milliseconds because the game session placement request may take some time to finish
			DescribeGameSessionPlacement(PlacementId);
			DescribeGameSessionPlacementEvent->Wait();
			if (AttemptToJoinGameFinished) {
				break;
			}
		}
		// went through 10 describe game session placement requests and did not join a game yet
		if (!AttemptToJoinGameFinished) {
			FailedToJoinGame = true;
			SucceededToJoinGame = false;
		}
	}
	else if(Status == 1 && GameSessionId.Len() > 0) {
		// very rare that a game session is made right after a call to startgamesessionplacement, will take some time
		const FString& PlayerSessionId = GenerateRandomId();
		//UE_LOG(LogMyMainMenu, Log, TEXT("on start game session placement success Game session id %s"), *GameSessionId);
		CreatePlayerSession(GameSessionId, PlayerSessionId);
		CreatePlayerSessionEvent->Wait();
	}
	StartGameSessionPlacementEvent->Trigger();
}

void UMainMenuWidget::OnStartGameSessionPlacementFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on start game session placement failed %s"), *ErrorMessage);
	FailedToJoinGame = true;
	SucceededToJoinGame = false;
	StartGameSessionPlacementEvent->Trigger();
}

void UMainMenuWidget::DescribeGameSessionPlacement(const FString& PlacementId) {
	UGameLiftDescribeGameSessionPlacement* DescribeGameSessionPlacementObject = Client->DescribeGameSessionPlacement(PlacementId);
	DescribeGameSessionPlacementObject->OnDescribeGameSessionPlacementSuccess.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionPlacementSuccess);
	DescribeGameSessionPlacementObject->OnDescribeGameSessionPlacementFailed.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionPlacementFailed);
	DescribeGameSessionPlacementObject->Activate();
}

void UMainMenuWidget::OnDescribeGameSessionPlacementSuccess(const FString& GameSessionId, const FString& PlacementId, const int& Status) {
	/*UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session id %s"), *GameSessionId);
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session placement id %s"), *PlacementId);
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session placement status %s"), *FString::FromInt(Status));*/
	if (Status < 0) {
		AttemptToJoinGameFinished = true; // game session placement failed
		SucceededToJoinGame = false;
		FailedToJoinGame = true;
	}
	else if (Status < 1 || GameSessionId.Len() <= 0) {
		// game session placement still in pending status
	}
	else {
		const FString& PlayerSessionId = GenerateRandomId();
		//UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session id %s"), *GameSessionId);
		CreatePlayerSession(GameSessionId, PlayerSessionId);
		CreatePlayerSessionEvent->Wait();
	}
	DescribeGameSessionPlacementEvent->Trigger();
}

void UMainMenuWidget::OnDescribeGameSessionPlacementFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement failed %s"), *ErrorMessage);
	DescribeGameSessionPlacementEvent->Trigger();
}

FString UMainMenuWidget::GenerateRandomId() {
	int RandOne = FMath::RandRange(0, 200000);
	int RandTwo = FMath::RandRange(0, 200000);
	int RandThree = FMath::RandRange(0, 200000);

	FTimespan Timespan = UKismetMathLibrary::GetTimeOfDay(FDateTime::Now());
	double MilliSeconds = Timespan.GetTotalMilliseconds();

	FString RandOneString = FString::FromInt(RandOne);
	FString RandTwoString = FString::FromInt(RandTwo);
	FString RandThreeString = FString::FromInt(RandThree);
	FString MilliSecondsString = FString::SanitizeFloat(MilliSeconds);
	//UE_LOG(LogMyMainMenu, Log, TEXT("RandOne %s RandTwo %s RandTree %s Current milliseconds %s"),*RandOneString, *RandTwoString, *RandThreeString, *MilliSecondsString);

	RandOne = MilliSeconds - RandOne;
	RandTwo = MilliSeconds - RandTwo;
	RandThree = MilliSeconds - RandThree;
	FString Id = FString::FromInt(RandOne) + FString("-") + FString::FromInt(RandTwo) + FString("-") + FString::FromInt(RandThree);

	return Id;
}

void UMainMenuWidget::DisableMouseEvents() {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void UMainMenuWidget::EnableMouseEvents() {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
}

void UMainMenuWidget::ExitGame() {
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController != nullptr) {
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

void UMainMenuWidget::LaunchDiscord() {
	const FString& DiscordUrl = "https://discord.gg/2xbR5qT";
	UKismetSystemLibrary::LaunchURL(DiscordUrl);
}

void UMainMenuWidget::LaunchYoutube() {
	const FString& YoutubeUrl = "https://www.youtube.com/channel/UCO77KLKwplncMHLo6gLpIHw";
	UKismetSystemLibrary::LaunchURL(YoutubeUrl);
}

void UMainMenuWidget::LaunchTwitter() {
	const FString& TwitterUrl = "https://twitter.com/flopperam";
	UKismetSystemLibrary::LaunchURL(TwitterUrl);
}

void UMainMenuWidget::LaunchPatreon() {
	const FString& PatreonUrl = "https://www.patreon.com/flopperam";
	UKismetSystemLibrary::LaunchURL(PatreonUrl);
}

void UMainMenuWidget::LaunchInstagram() {
	const FString& InstagramUrl = "https://www.instagram.com/flopperam/";
	UKismetSystemLibrary::LaunchURL(InstagramUrl);
}

void UMainMenuWidget::LaunchFacebook() {
	const FString& FacebookUrl = "https://www.facebook.com/flopperam/";
	UKismetSystemLibrary::LaunchURL(FacebookUrl);
}
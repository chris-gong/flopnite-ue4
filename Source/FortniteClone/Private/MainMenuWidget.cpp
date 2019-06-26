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

	AttemptToJoinGameFinished = false;
	GameSessionsLeft = 4;
}

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();
	// line below is for testing local gamelift
	//Client = UGameLiftClientObject::CreateGameLiftObject(AccessKey, SecretKey, "us-east-1", true, 9080);
	Client = UGameLiftClientObject::CreateGameLiftObject(AccessKey, SecretKey, "us-east-1");
	JoinGameButton = (UButton*) GetWidgetFromName(TEXT("Button_JoinGame"));
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinGame);
}
void UMainMenuWidget::JoinGame() {
	AttemptToJoinGameFinished = false;
	JoinGameButton->SetIsEnabled(false);
	DescribeGameSessionQueues(QueueName);
}

void UMainMenuWidget::DescribeGameSessionQueues(const FString& QueueNameInput) {
	UGameLiftDescribeGameSessionQueues* DescribeGameSessionQueuesObject = Client->DescribeGameSessionQueues(QueueNameInput);
	DescribeGameSessionQueuesObject->OnDescribeGameSessionQueuesSuccess.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionQueuesSuccess);
	DescribeGameSessionQueuesObject->OnDescribeGameSessionQueuesFailed.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionQueuesFailed);
	DescribeGameSessionQueuesObject->Activate();
	UE_LOG(LogMyMainMenu, Log, TEXT("describe game session queues activated"));
}

void UMainMenuWidget::OnDescribeGameSessionQueuesSuccess(const TArray<FString>& FleetARNs) {
	UE_LOG(LogMyMainMenu, Log, TEXT("describe game session queues success"));
	for (int i = 0; i < FleetARNs.Num(); i++) {
		FString FleetArn = FleetARNs[i];
		UE_LOG(LogMyMainMenu, Log, TEXT("%s"), *FleetArn);
		TArray<FString> FleetArnParsedOnce;
		FleetArn.ParseIntoArray(FleetArnParsedOnce, TEXT("arn:aws:gamelift:"), true);
		TArray<FString> FleetArnParsedAgain;
		FleetArnParsedOnce[0].ParseIntoArray(FleetArnParsedAgain, TEXT("::fleet/"), true);

		const FString& FleetId = FleetArnParsedAgain[1];
		SearchGameSessions(FleetId);
	}
}

void UMainMenuWidget::OnDescribeGameSessionQueuesFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session queues failed %s"), *ErrorMessage);
	AttemptToJoinGameFinished = true;
	JoinGameButton->SetIsEnabled(true);
}

void UMainMenuWidget::SearchGameSessions(const FString& FleetId) {
	UGameLiftSearchGameSessions* SearchGameSessionsObject = Client->SearchGameSessions(FleetId, "", "", ""); // TODO: Have to handle other parameters
	SearchGameSessionsObject->OnSearchGameSessionsSuccess.AddDynamic(this, &UMainMenuWidget::OnSearchGameSessionsSuccess);
	SearchGameSessionsObject->OnSearchGameSessionsFailed.AddDynamic(this, &UMainMenuWidget::OnSearchGameSessionsFailed);
	SearchGameSessionsObject->Activate();
}
void UMainMenuWidget::OnSearchGameSessionsSuccess(const TArray<FString>& GameSessionIds) {
	//SearchGameSessionsFinished = true;
	UE_LOG(LogMyMainMenu, Log, TEXT("on search game session success"));
	GameSessionsLeft = GameSessionIds.Num();

	if (GameSessionsLeft <= 0) {
		AttemptToJoinGameFinished = true;

		const FString& PlacementId = GenerateRandomId();
		StartGameSessionPlacement(QueueName, 100, PlacementId);
	}
	else {
		for (int i = 0; i < GameSessionIds.Num(); i++) {
			const FString& GameSessionId = GameSessionIds[i];
			const FString& PlayerSessionId = GenerateRandomId();
			UE_LOG(LogMyMainMenu, Log, TEXT("on search game session success Game session id %s"), *GameSessionId);
			CreatePlayerSession(GameSessionId, PlayerSessionId);
		}
	}
}

void UMainMenuWidget::OnSearchGameSessionsFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on search game sessions failed %s"), *ErrorMessage);
	AttemptToJoinGameFinished = true;
	JoinGameButton->SetIsEnabled(true);
}

void UMainMenuWidget::CreatePlayerSession(const FString& GameSessionId, const FString& PlayerSessionId) {
	UGameLiftCreatePlayerSession* CreatePlayerSessionObject = Client->CreatePlayerSession(GameSessionId, PlayerSessionId);
	CreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UMainMenuWidget::OnCreatePlayerSessionSuccess);
	CreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UMainMenuWidget::OnCreatePlayerSessionFailed);
	CreatePlayerSessionObject->Activate();
}

void UMainMenuWidget::OnCreatePlayerSessionSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID, const int& PlayerSessionStatus) {
	FString PlayerSessionStatusString = FString::FromInt(PlayerSessionStatus);
	UE_LOG(LogMyMainMenu, Log, TEXT("player session status %s"), *PlayerSessionStatusString);
	if (PlayerSessionStatus == 1) {
		FString LevelName = IPAddress + FString(":") + Port;
		const FString& Options = FString("?") + FString("PlayerSessionId=") + PlayerSessionID;

		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), false, Options);
	}
	else if (PlayerSessionStatus == 2) {
		// already activated?
	}

	JoinGameButton->SetIsEnabled(true);
}

void UMainMenuWidget::OnCreatePlayerSessionFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on create player session failed %s"), *ErrorMessage);
	GameSessionsLeft -= 1;
	if (!AttemptToJoinGameFinished && GameSessionsLeft <= 0) {
		AttemptToJoinGameFinished = true;

		const FString& PlacementId = GenerateRandomId();
		StartGameSessionPlacement(QueueName, 100, PlacementId);
	}
	if (AttemptToJoinGameFinished) {
		JoinGameButton->SetIsEnabled(true);
	}
}

void UMainMenuWidget::StartGameSessionPlacement(const FString& QueueNameInput, const int& MaxPlayerCount, const FString& PlacementId) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("start game session placement called in main menu widget"));
	UGameLiftStartGameSessionPlacement* StartGameSessionPlacementObject = Client->StartGameSessionPlacement(QueueNameInput, MaxPlayerCount, PlacementId);
	StartGameSessionPlacementObject->OnStartGameSessionPlacementSuccess.AddDynamic(this, &UMainMenuWidget::OnStartGameSessionPlacementSuccess);
	StartGameSessionPlacementObject->OnStartGameSessionPlacementFailed.AddDynamic(this, &UMainMenuWidget::OnStartGameSessionPlacementFailed);
	StartGameSessionPlacementObject->Activate();
}

void UMainMenuWidget::OnStartGameSessionPlacementSuccess(const FString& GameSessionId, const FString& PlacementId) {
	if (GameSessionId.Len() <= 0) {
		if (PlacementId.Len() > 0) {
			DescribeGameSessionPlacement(PlacementId);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString("Either a new game session was just made and you have to click join game again, or there are no available game sessions currently"));
			JoinGameButton->SetIsEnabled(true);
		}
	}
	else {
		const FString& PlayerSessionId = GenerateRandomId();
		UE_LOG(LogMyMainMenu, Log, TEXT("on start game session placement success Game session id %s"), *GameSessionId);
		CreatePlayerSession(GameSessionId, PlayerSessionId);
	}
}

void UMainMenuWidget::OnStartGameSessionPlacementFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on start game session placement failed %s"), *ErrorMessage);
	JoinGameButton->SetIsEnabled(true);
}

void UMainMenuWidget::DescribeGameSessionPlacement(const FString& PlacementId) {
	UGameLiftDescribeGameSessionPlacement* DescribeGameSessionPlacementObject = Client->DescribeGameSessionPlacement(PlacementId);
	DescribeGameSessionPlacementObject->OnDescribeGameSessionPlacementSuccess.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionPlacementSuccess);
	DescribeGameSessionPlacementObject->OnDescribeGameSessionPlacementFailed.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionPlacementFailed);
	DescribeGameSessionPlacementObject->Activate();
}

void UMainMenuWidget::OnDescribeGameSessionPlacementSuccess(const FString& GameSessionId, const FString& PlacementId, const int& Status) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session id %s"), *GameSessionId);
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session placement id %s"), *PlacementId);
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session placement status %s"), *FString::FromInt(Status));
	if (Status < 0 || GameSessionId.Len() <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString("Either a new game session was just made and you have to click join game again, or there are no available game sessions currently"));
		JoinGameButton->SetIsEnabled(true);
	}
	else {
		const FString& PlayerSessionId = GenerateRandomId();
		UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement success Game session id %s"), *GameSessionId);
		CreatePlayerSession(GameSessionId, PlayerSessionId);
	}
}

void UMainMenuWidget::OnDescribeGameSessionPlacementFailed(const FString& ErrorMessage) {
	UE_LOG(LogMyMainMenu, Log, TEXT("on describe game session placement failed %s"), *ErrorMessage);
	JoinGameButton->SetIsEnabled(true);
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
	UE_LOG(LogMyMainMenu, Log, TEXT("RandOne %s RandTwo %s RandTree %s Current milliseconds %s"),*RandOneString, *RandTwoString, *RandThreeString, *MilliSecondsString);

	RandOne = MilliSeconds - RandOne;
	RandTwo = MilliSeconds - RandTwo;
	RandThree = MilliSeconds - RandThree;
	FString Id = FString::FromInt(RandOne) + FString("-") + FString::FromInt(RandTwo) + FString("-") + FString::FromInt(RandThree);

	return Id;
}
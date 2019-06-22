// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TextReaderComponent.h"
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#include "GameLiftClientSDK/Public/GameLiftClientApi.h"
#include "Engine/Engine.h"
#include "Runtime\Engine\Classes\Kismet\KismetMathLibrary.h"

// Add default functionality here for any IMainMenuWidget functions that are not pure virtual.

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));
	AccessKey = TextReader->ReadFile("Credentials/AWS_AccessKey.txt");
	SecretKey = TextReader->ReadFile("Credentials/AWS_SecretKey.txt");
	QueueName = TextReader->ReadFile("Credentials/AWS_QueueName.txt");

	Client = UGameLiftClientObject::CreateGameLiftObject(AccessKey, SecretKey, "us-east-1");
	SearchGameSessionsFinished = false;
	CreatePlayerSessionFinished = false;
}

void UMainMenuWidget::JoinGame() {
	DescribeGameSessionQueues(QueueName);
}

void UMainMenuWidget::DescribeGameSessionQueues(const FString& QueueNameInput) {
	UGameLiftDescribeGameSessionQueues* DescribeGameSessionQueuesObject = Client->DescribeGameSessionQueues(QueueNameInput);
	DescribeGameSessionQueuesObject->OnDescribeGameSessionQueuesSuccess.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionQueuesSuccess);
	DescribeGameSessionQueuesObject->OnDescribeGameSessionQueuesFailed.AddDynamic(this, &UMainMenuWidget::OnDescribeGameSessionQueuesFailed);
	DescribeGameSessionQueuesObject->Activate();
}

void UMainMenuWidget::OnDescribeGameSessionQueuesSuccess(const TArray<FString>& FleetARNs) {
	for (int i = 0; i < FleetARNs.Num(); i++) {
		FString FleetArn = FleetARNs[i];
		TArray<FString> FleetArnParsedOnce;
		FleetArn.ParseIntoArray(FleetArnParsedOnce, TEXT("arn:aws:gamelift:"), true);
		TArray<FString> FleetArnParsedAgain;
		FleetArnParsedOnce[1].ParseIntoArray(FleetArnParsedAgain, TEXT("::fleet/"), true);

		FString FleetId = FleetArnParsedAgain[1];
		SearchGameSessions(FleetId);

		while (!SearchGameSessionsFinished) {
			// block this function until search game sessions finishes
		}

		SearchGameSessionsFinished = false; // reset variable for next iteration
	}
}

void UMainMenuWidget::OnDescribeGameSessionQueuesFailed(const FString& ErrorMessage) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);
}

void UMainMenuWidget::SearchGameSessions(const FString& FleetId) {
	UGameLiftSearchGameSessions* SearchGameSessionsObject = Client->SearchGameSessions(FleetId, "", "", ""); // TODO: Have to handle other parameters
	SearchGameSessionsObject->OnSearchGameSessionsSuccess.AddDynamic(this, &UMainMenuWidget::OnSearchGameSessionsSuccess);
	SearchGameSessionsObject->OnSearchGameSessionsFailed.AddDynamic(this, &UMainMenuWidget::OnSearchGameSessionsFailed);
	SearchGameSessionsObject->Activate();
}
void UMainMenuWidget::OnSearchGameSessionsSuccess(const TArray<FString>& GameSessionIds) {
	//SearchGameSessionsFinished = true;
	for (int i = 0; i < GameSessionIds.Num(); i++) {
		FString GameSessionId = GameSessionIds[i];
		FString PlayerSessionId = GenerateRandomId();
		CreatePlayerSession(GameSessionId, PlayerSessionId);

		while (!CreatePlayerSessionFinished) {
			// block this function until create player session finishes
		}

		CreatePlayerSessionFinished = false; // reset variable for next iteration
	}
	SearchGameSessionsFinished = true;
}

void UMainMenuWidget::OnSearchGameSessionsFailed(const FString& ErrorMessage) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);
	SearchGameSessionsFinished = true;
}

void UMainMenuWidget::CreatePlayerSession(const FString& GameSessionId, const FString& PlayerSessionId) {
	UGameLiftCreatePlayerSession* CreatePlayerSessionObject = Client->CreatePlayerSession(GameSessionId, PlayerSessionId);
	CreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UMainMenuWidget::OnCreatePlayerSessionSuccess);
	CreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UMainMenuWidget::OnCreatePlayerSessionFailed);
	CreatePlayerSessionObject->Activate();
}

void UMainMenuWidget::OnCreatePlayerSessionSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID, const int& PlayerSessionStatus) {
	CreatePlayerSessionFinished = true;
}

void UMainMenuWidget::OnCreatePlayerSessionFailed(const FString& ErrorMessage) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);
	CreatePlayerSessionFinished = true;
}

FString UMainMenuWidget::GenerateRandomId() {
	int RandOne = FMath::RandRange(0, 1000);
	int RandTwo = FMath::RandRange(0, 1000);
	int RandThree = FMath::RandRange(0, 1000);

	FTimespan Timespan = UKismetMathLibrary::GetTimeOfDay(FDateTime::Now());
	double MilliSeconds = Timespan.GetTotalMilliseconds();
	RandOne *= MilliSeconds;
	RandTwo *= MilliSeconds;
	RandThree *= MilliSeconds;
	FString Id = FString::FromInt(RandOne) + FString("-") + FString::FromInt(RandTwo) + FString("-") + FString::FromInt(RandThree);

	return Id;
}
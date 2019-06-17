#include "FortniteCloneGameInstance.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "StormActor.h"
#if WITH_GAMELIFTCLIENTSDK
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#include "GameLiftClientSDK/Public/GameLiftClientApi.h"
#endif

void UFortniteCloneGameInstance::Init()
{
	Super::Init();
	//GetWorld()->SpawnActor<AStormActor>(AStormActor::StaticClass(), FVector(-440, -1450, 10000), FRotator::ZeroRotator);
#if WITH_GAMELIFTCLIENTSDK
	// Create the game lift object. This is required before calling any GameLift functions.
	GameLiftClientObject = UGameLiftClientObject::CreateGameLiftObject("", "", "us-east-2");
	//CreateGameSession();
#endif
}

void UFortniteCloneGameInstance::CreateGameSession()
{
#if WITH_GAMELIFTCLIENTSDK
	FGameLiftGameSessionConfig MySessionConfig;
	MySessionConfig.SetAliasID("Your Alias ID");
	MySessionConfig.SetMaxPlayers(10);
	UGameLiftCreateGameSession* MyGameSessionObject = GameLiftClientObject->CreateGameSession(MySessionConfig);
	MyGameSessionObject->OnCreateGameSessionSuccess.AddDynamic(this, &UFortniteCloneGameInstance::OnGameCreationSuccess);
	MyGameSessionObject->OnCreateGameSessionFailed.AddDynamic(this, &UFortniteCloneGameInstance::OnGameCreationFailed);
	MyGameSessionObject->Activate();
#endif
}

void UFortniteCloneGameInstance::OnGameCreationSuccess(const FString& GameSessionID)
{
	DescribeGameSession(GameSessionID);
}

void UFortniteCloneGameInstance::OnGameCreationFailed(const FString& ErrorMessage)
{
#if WITH_GAMELIFTCLIENTSDK
	// Do stuff...
#endif
}

void UFortniteCloneGameInstance::DescribeGameSession(const FString& GameSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSession* MyDescribeGameSessionObject = GameLiftClientObject->DescribeGameSession(GameSessionID);
	MyDescribeGameSessionObject->OnDescribeGameSessionStateSuccess.AddDynamic(this, &UFortniteCloneGameInstance::OnDescribeGameSessionSuccess);
	MyDescribeGameSessionObject->OnDescribeGameSessionStateFailed.AddDynamic(this, &UFortniteCloneGameInstance::OnDescribeGameSessionFailed);
	MyDescribeGameSessionObject->Activate();
#endif
}

void UFortniteCloneGameInstance::OnDescribeGameSessionSuccess(const FString& SessionID, EGameLiftGameSessionStatus SessionState)
{
	// Player sessions can only be created on ACTIVE instance.
	if (SessionState == EGameLiftGameSessionStatus::STATUS_Active)
	{
		CreatePlayerSession(SessionID, "Your Unique Player ID");
	}
}

void UFortniteCloneGameInstance::OnDescribeGameSessionFailed(const FString& ErrorMessage)
{
#if WITH_GAMELIFTCLIENTSDK
	// Do stuff...
#endif
}

void UFortniteCloneGameInstance::CreatePlayerSession(const FString& GameSessionID, const FString UniquePlayerID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSession* MyCreatePlayerSessionObject = GameLiftClientObject->CreatePlayerSession(GameSessionID, UniquePlayerID);
	MyCreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UFortniteCloneGameInstance::OnPlayerSessionCreateSuccess);
	MyCreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UFortniteCloneGameInstance::OnPlayerSessionCreateFail);
	MyCreatePlayerSessionObject->Activate();
#endif
}

void UFortniteCloneGameInstance::OnPlayerSessionCreateSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	const FString TravelURL = IPAddress + ":" + Port;
	UGameplayStatics::GetPlayerController(this, 0)->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
#endif
}

void UFortniteCloneGameInstance::OnPlayerSessionCreateFail(const FString& ErrorMessage)
{
#if WITH_GAMELIFTCLIENTSDK
	// Do stuff...
#endif
}